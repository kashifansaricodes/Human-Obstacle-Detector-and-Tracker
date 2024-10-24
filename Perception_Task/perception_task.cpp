/**
 * @file perception_task.cpp
 * @brief Implementation of the HumanDetectorTracker class for detecting and
 * tracking humans in video frames.
 */

#include "perception_task.hpp"

#include <fstream>
#include <iostream>

/**
 * @brief Constructor for HumanDetectorTracker.
 * @param modelPath Path to the YOLO model weights file.
 * @param configPath Path to the YOLO model configuration file.
 * @param classesPath Path to the COCO classes file.
 * @throw std::runtime_error if the model fails to load.
 */
HumanDetectorTracker::HumanDetectorTracker(const std::string& modelPath,
                                           const std::string& configPath,
                                           const std::string& classesPath) {
  net = cv::dnn::readNetFromDarknet(configPath, modelPath);
  if (net.empty()) {
    throw std::runtime_error("Failed to load the model from: " + modelPath);
  }
  net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
  net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

  // Load class names
  std::ifstream classNamesFile(classesPath);
  if (classNamesFile.is_open()) {
    std::string className;
    while (std::getline(classNamesFile, className)) {
      classes.push_back(className);
    }
  }

  // Initialize camera matrix and distortion coefficients
  cameraMatrix =
      (cv::Mat_<double>(3, 3) << 1000, 0, 320, 0, 1000, 240, 0, 0, 1);
  distCoeffs = cv::Mat::zeros(5, 1, CV_64F);
}

/**
 * @brief Detects humans in a given frame using YOLO.
 * @param frame The input frame to process.
 * @return A vector of bounding boxes for detected humans.
 */
std::vector<cv::Rect> HumanDetectorTracker::detectHumans(const cv::Mat& frame) {
  cv::Mat blob = cv::dnn::blobFromImage(frame, 1 / 255.0, cv::Size(416, 416),
                                        cv::Scalar(0, 0, 0), true, false);
  net.setInput(blob);

  std::vector<cv::Mat> outs;
  net.forward(outs, net.getUnconnectedOutLayersNames());

  std::vector<cv::Rect> boxes;
  std::vector<float> confidences;

  for (const auto& out : outs) {
    for (int i = 0; i < out.rows; ++i) {
      cv::Mat scores = out.row(i).colRange(5, out.cols);
      cv::Point classIdPoint;
      double confidence;
      cv::minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);

      if (confidence > 0.5 && classes[classIdPoint.x] == "person") {
        int centerX = static_cast<int>(out.at<float>(i, 0) * frame.cols);
        int centerY = static_cast<int>(out.at<float>(i, 1) * frame.rows);
        int width = static_cast<int>(out.at<float>(i, 2) * frame.cols);
        int height = static_cast<int>(out.at<float>(i, 3) * frame.rows);
        int left = centerX - width / 2;
        int top = centerY - height / 2;

        boxes.push_back(cv::Rect(left, top, width, height));
        confidences.push_back(static_cast<float>(confidence));
      }
    }
  }

  std::vector<int> indices;
  cv::dnn::NMSBoxes(boxes, confidences, 0.5, 0.4, indices);

  std::vector<cv::Rect> detectedHumans;
  for (int idx : indices) {
    detectedHumans.push_back(boxes[idx]);
  }

  return detectedHumans;
}

/**
 * @brief Detects and tracks humans in a given frame.
 * @param frame The input frame to process.
 */
void HumanDetectorTracker::detectAndTrack(cv::Mat& frame) {
  std::vector<cv::Rect> detections = detectHumans(frame);
  updateTrackers(frame, detections);

  // Draw bounding boxes for detected humans
  for (const auto& detection : detections) {
    cv::rectangle(frame, detection, cv::Scalar(0, 255, 0), 2);
    cv::Point3f location = getLocation(detection);
    cv::putText(frame,
                "Human: (" + std::to_string(static_cast<int>(location.x)) +
                    ", " + std::to_string(static_cast<int>(location.y)) + ", " +
                    std::to_string(static_cast<int>(location.z)) + ")",
                cv::Point(detection.x, detection.y - 10),
                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
  }
}

/**
 * @brief Updates the trackers with new detections.
 * @param frame The current frame.
 * @param detections The new detections to update the trackers with.
 */
void HumanDetectorTracker::updateTrackers(
    cv::Mat& frame, const std::vector<cv::Rect>& detections) {
  // Update existing trackers
  auto it = trackers.begin();
  while (it != trackers.end()) {
    cv::Rect trackedRect;
    if ((*it)->update(frame, trackedRect)) {
      cv::rectangle(frame, trackedRect, cv::Scalar(255, 0, 0), 2);
      cv::Point3f location = getLocation(trackedRect);
      cv::putText(frame,
                  "Tracked: (" + std::to_string(static_cast<int>(location.x)) +
                      ", " + std::to_string(static_cast<int>(location.y)) +
                      ", " + std::to_string(static_cast<int>(location.z)) + ")",
                  cv::Point(trackedRect.x, trackedRect.y - 10),
                  cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 2);
      ++it;
    } else {
      it = trackers.erase(it);
    }
  }

  // Initialize new trackers for newly detected humans
  for (const auto& det : detections) {
    bool isNewDetection = true;
    for (const auto& tracker : trackers) {
      cv::Rect trackedRect;
      tracker->update(frame, trackedRect);
      if ((det & trackedRect).area() > 0) {
        isNewDetection = false;
        break;
      }
    }

    if (isNewDetection) {
      cv::Ptr<cv::Tracker> tracker = cv::TrackerKCF::create();
      tracker->init(frame, det);
      trackers.push_back(tracker);
    }
  }
}

/**
 * @brief Estimates the 3D location of a detected object.
 * @param rect The bounding box of the detected object.
 * @return The estimated 3D location as a cv::Point3f.
 */
cv::Point3f HumanDetectorTracker::getLocation(const cv::Rect& rect) {
  // Estimate 3D position (x, y, z) in robot's reference frame
  // This is a simplified calculation and would need to be adjusted based on
  // your specific setup
  double focalLength = cameraMatrix.at<double>(0, 0);
  double centerX = cameraMatrix.at<double>(0, 2);
  double centerY = cameraMatrix.at<double>(1, 2);

  double x = (rect.x + rect.width / 2 - centerX) / focalLength;
  double y = (rect.y + rect.height / 2 - centerY) / focalLength;
  double z = focalLength / rect.width;  // Assuming known average human width

  return cv::Point3f(x, y, z);
}

#include "../include/perception_task.hpp"
#include <fstream>
#include <iostream>

HumanDetectorTracker::HumanDetectorTracker(const std::string& modelPath, const std::string& classesPath) {
    // Load YOLO network from ONNX file
    net = cv::dnn::readNetFromONNX(modelPath);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    // Load class names
    std::ifstream ifs(classesPath.c_str());
    std::string line;
    while (std::getline(ifs, line)) classes.push_back(line);

    // Initialize camera matrix and distortion coefficients
    // These should be calibrated for your specific camera
    cameraMatrix = (cv::Mat_<double>(3,3) << 
        1000, 0, 320,
        0, 1000, 240,
        0, 0, 1);
    distCoeffs = cv::Mat::zeros(5, 1, CV_64F);
}

void HumanDetectorTracker::detectAndTrack(cv::Mat& frame) {
    std::vector<cv::Rect> detections = detectHumans(frame);
    updateTrackers(frame, detections);
}

std::vector<cv::Rect> HumanDetectorTracker::detectHumans(const cv::Mat& frame) {
    cv::Mat blob;
    cv::dnn::blobFromImage(frame, blob, 1/255.0, cv::Size(640, 640), cv::Scalar(0,0,0), true, false);
    net.setInput(blob);

    std::vector<cv::Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());

    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;

    for (const auto& output : outputs) {
        for (int i = 0; i < output.rows; ++i) {
            cv::Mat scores = output.row(i).colRange(5, output.cols);
            cv::Point classIdPoint;
            double confidence;
            cv::minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            
            if (confidence > 0.5 && classes[classIdPoint.x] == "person") {
                float centerX = output.at<float>(i, 0) * frame.cols;
                float centerY = output.at<float>(i, 1) * frame.rows;
                float width = output.at<float>(i, 2) * frame.cols;
                float height = output.at<float>(i, 3) * frame.rows;
                
                cv::Rect box(centerX - width/2, centerY - height/2, width, height);
                boxes.push_back(box);
                classIds.push_back(classIdPoint.x);
                confidences.push_back(static_cast<float>(confidence));
            }
        }
    }

    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, 0.5, 0.4, indices);

    std::vector<cv::Rect> detections;
    for (int idx : indices) {
        detections.push_back(boxes[idx]);
    }

    return detections;
}

void HumanDetectorTracker::updateTrackers(cv::Mat& frame, const std::vector<cv::Rect>& detections) {
    // Update existing trackers
    auto it = trackers.begin();
    while (it != trackers.end()) {
        cv::Rect trackedRect;
        if ((*it)->update(frame, trackedRect)) {
            cv::rectangle(frame, trackedRect, cv::Scalar(0, 255, 0), 2);
            cv::Point3f location = getLocation(trackedRect);
            std::cout << "Tracked human at: (" << location.x << ", " << location.y << ", " << location.z << ")" << std::endl;
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
            cv::rectangle(frame, det, cv::Scalar(255, 0, 0), 2);
            cv::Point3f location = getLocation(det);
            std::cout << "New human detected at: (" << location.x << ", " << location.y << ", " << location.z << ")" << std::endl;
        }
    }
}

cv::Point3f HumanDetectorTracker::getLocation(const cv::Rect& rect) {
    // Estimate 3D position (x, y, z) in robot's reference frame
    // This is a simplified calculation and would need to be adjusted based on your specific setup
    double focalLength = cameraMatrix.at<double>(0, 0);
    double centerX = cameraMatrix.at<double>(0, 2);
    double centerY = cameraMatrix.at<double>(1, 2);
    
    double x = (rect.x + rect.width/2 - centerX) / focalLength;
    double y = (rect.y + rect.height/2 - centerY) / focalLength;
    double z = focalLength / rect.width;  // Assuming known average human width

    return cv::Point3f(x, y, z);
}
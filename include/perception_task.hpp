#ifndef PERCEPTION_TASK_HPP
#define PERCEPTION_TASK_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <string>
#include <vector>

/**
 * @class HumanDetectorTracker
 * @brief A class for detecting and tracking humans in video frames.
 */
class HumanDetectorTracker {
 public:
  /**
   * @brief Constructor for HumanDetectorTracker.
   * @param modelPath Path to the YOLO model weights file.
   * @param configPath Path to the YOLO model configuration file.
   * @param classesPath Path to the COCO classes file.
   */
  HumanDetectorTracker(const std::string& modelPath,
                       const std::string& configPath,
                       const std::string& classesPath);

  /**
   * @brief Detects and tracks humans in a given frame.
   * @param frame The input frame to process.
   */
  void detectAndTrack(cv::Mat& frame);

 private:
  cv::dnn::Net net;  ///< The neural network for object detection.
  std::vector<std::string> classes;  ///< List of object classes.
  cv::Mat cameraMatrix;              ///< Camera intrinsic parameters.
  cv::Mat distCoeffs;                ///< Camera distortion coefficients.
  std::vector<cv::Ptr<cv::Tracker>> trackers;  ///< List of object trackers.

  /**
   * @brief Estimates the 3D location of a detected object.
   * @param rect The bounding box of the detected object.
   * @return The estimated 3D location.
   */
  cv::Point3f getLocation(const cv::Rect& rect);

  /**
   * @brief Detects humans in a given frame.
   * @param frame The input frame to process.
   * @return A vector of bounding boxes for detected humans.
   */
  std::vector<cv::Rect> detectHumans(const cv::Mat& frame);

  /**
   * @brief Updates the trackers with new detections.
   * @param frame The current frame.
   * @param detections The new detections to update the trackers with.
   */
  void updateTrackers(cv::Mat& frame, const std::vector<cv::Rect>& detections);
};

#endif  // PERCEPTION_TASK_HPP
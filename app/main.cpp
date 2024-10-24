#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#include "perception_task.hpp"

/**
 * @brief Main function for the Human Obstacle Detector and Tracker
 *
 * This program initializes a camera capture, sets up the YOLO model for human
 * detection, and continuously processes video frames to detect and track
 * humans.
 *
 * @return int Returns 0 on successful execution, -1 on camera opening failure
 */
int main() {
  cv::VideoCapture cap(0);  // Open the default camera
  if (!cap.isOpened()) {
    std::cerr << "Error opening video capture" << std::endl;
    return -1;
  }

  // Use absolute paths
  std::string model_path =
      "/home/abhishek/Human-Obstacle-Detector-and-Tracker/yolo_files/"
      "yolov3.weights";
  std::string config_path =
      "/home/abhishek/Human-Obstacle-Detector-and-Tracker/yolo_files/"
      "yolov3.cfg";
  std::string classes_path =
      "/home/abhishek/Human-Obstacle-Detector-and-Tracker/yolo_files/"
      "coco.names";

  std::cout << "Model path: " << model_path << std::endl;
  std::cout << "Config path: " << config_path << std::endl;
  std::cout << "Classes path: " << classes_path << std::endl;

  try {
    /**
     * @brief Initialize the HumanDetectorTracker object
     *
     * @param model_path Path to the YOLO model weights file
     * @param config_path Path to the YOLO model configuration file
     * @param classes_path Path to the COCO classes file
     */
    HumanDetectorTracker detector(model_path, config_path, classes_path);
    cv::Mat frame;

    /**
     * @brief Main processing loop
     *
     * Continuously captures frames from the camera, processes them for human
     * detection and tracking, and displays the results.
     */
    while (true) {
      cap >> frame;
      if (frame.empty()) {
        std::cerr << "Error capturing frame" << std::endl;
        break;
      }

      /**
       * @brief Detect and track humans in the current frame
       *
       * @param frame The current video frame to process
       */
      detector.detectAndTrack(frame);

      /**
       * @brief Display the processed frame
       *
       * @param "Human Detector and Tracker" Window name
       * @param frame The processed frame to display
       */
      cv::imshow("Human Detector and Tracker", frame);

      if (cv::waitKey(30) >= 0) break;
    }
  } catch (const cv::Exception& e) {
    std::cerr << "OpenCV exception: " << e.what() << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Standard exception: " << e.what() << std::endl;
  }

  return 0;
}

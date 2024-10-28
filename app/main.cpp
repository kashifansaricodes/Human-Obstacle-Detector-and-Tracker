#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <chrono>

#include "perception_task.hpp"

/**
 * @brief Main function for the Human Obstacle Detector and Tracker
 *
 * This program initializes a camera capture, sets up the YOLO model for human
 * detection, and processes video frames to detect and track humans for 10 seconds.
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
     * @brief Processing loop with 10-second duration
     *
     * Captures frames from the camera, processes them for human
     * detection and tracking, and displays the results for 10 seconds.
     */
    auto start_time = std::chrono::steady_clock::now();
    const auto duration = std::chrono::seconds(10);

    std::cout << "Starting detection for 10 seconds..." << std::endl;

    while (true) {
      // Check if 10 seconds have elapsed
      auto current_time = std::chrono::steady_clock::now();
      if (current_time - start_time >= duration) {
        std::cout << "10 seconds completed, exiting..." << std::endl;
        break;
      }

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

      // Display remaining time
      auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time);
      auto remaining = duration - elapsed;
      std::cout << "\rTime remaining: " << remaining.count() << " seconds" << std::flush;

      // Break if 'q' is pressed
      if (cv::waitKey(30) == 'q') break;
    }

    // Clean up
    cap.release();
    cv::destroyAllWindows();

  } catch (const cv::Exception& e) {
    std::cerr << "OpenCV exception: " << e.what() << std::endl;
    return -1;
  } catch (const std::exception& e) {
    std::cerr << "Standard exception: " << e.what() << std::endl;
    return -1;
  }

  return 0;
}
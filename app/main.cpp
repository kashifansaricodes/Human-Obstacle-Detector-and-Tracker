#include "perception_task.hpp"
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>

int main() {
    cv::VideoCapture cap(0);  // Open the default camera
    if (!cap.isOpened()) {
        std::cerr << "Error opening video capture" << std::endl;
        return -1;
    }

    // Use absolute paths
    std::string model_path = "/home/abhishek/Human-Obstacle-Detector-and-Tracker/yolo_files/yolov3.weights";
    std::string config_path = "/home/abhishek/Human-Obstacle-Detector-and-Tracker/yolo_files/yolov3.cfg";
    std::string classes_path = "/home/abhishek/Human-Obstacle-Detector-and-Tracker/yolo_files/coco.names";

    std::cout << "Model path: " << model_path << std::endl;
    std::cout << "Config path: " << config_path << std::endl;
    std::cout << "Classes path: " << classes_path << std::endl;

    try {
        HumanDetectorTracker detector(model_path, config_path, classes_path);
        cv::Mat frame;

        while (true) {
            cap >> frame;
            if (frame.empty()) {
                std::cerr << "Error capturing frame" << std::endl;
                break;
            }

            detector.detectAndTrack(frame);

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
#include "perception_task.hpp"
#include <opencv2/opencv.hpp>
#include <string>

int main() {
    cv::VideoCapture cap(0);  // Open the default camera
    if (!cap.isOpened()) {
        std::cerr << "Error opening video capture" << std::endl;
        return -1;
    }

    // Use relative paths
    std::string onnx_path = "../yolo_files/yolov5s.onnx";
    std::string coco_path = "../yolo_files/coco.names";

    HumanDetectorTracker detector(onnx_path, coco_path);
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

    return 0;
}
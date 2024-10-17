#ifndef PERCEPTION_TASK_HPP
#define PERCEPTION_TASK_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <vector>
#include <string>

class HumanDetectorTracker {
public:
    HumanDetectorTracker(const std::string& modelPath, const std::string& configPath, const std::string& classesPath);
    void detectAndTrack(cv::Mat& frame);

private:
    cv::dnn::Net net;
    std::vector<std::string> classes;
    cv::Mat cameraMatrix;
    cv::Mat distCoeffs;
    std::vector<cv::Ptr<cv::Tracker>> trackers;

    cv::Point3f getLocation(const cv::Rect& rect);
    std::vector<cv::Rect> detectHumans(const cv::Mat& frame);
    void updateTrackers(cv::Mat& frame, const std::vector<cv::Rect>& detections);
};

#endif // PERCEPTION_TASK_HPP
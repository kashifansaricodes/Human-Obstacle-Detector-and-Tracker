#include <gtest/gtest.h>
#include "perception_task.hpp"
#include <opencv2/opencv.hpp>

class PerceptionTaskTest : public ::testing::Test {
protected:
    HumanDetectorTracker* detector;
    std::string model_path;
    std::string config_path;
    std::string classes_path;

    void SetUp() override {
        // Set correct paths relative to project root
        model_path = "yolo_files/yolov3.weights";
        config_path = "yolo_files/yolov3.cfg";
        classes_path = "yolo_files/coco.names";
        
        std::cout << "\nChecking YOLO files:" << std::endl;
        std::cout << "Model path: " << model_path << std::endl;
        std::cout << "Config path: " << config_path << std::endl;
        std::cout << "Classes path: " << classes_path << std::endl;

        try {
            detector = new HumanDetectorTracker(model_path, config_path, classes_path);
        } catch (const std::exception& e) {
            detector = nullptr;
            std::cout << "Error initializing detector: " << e.what() << std::endl;
        }
    }

    void TearDown() override {
        delete detector;
        detector = nullptr;
    }
};

// Test basic initialization
TEST_F(PerceptionTaskTest, InitializationTest) {
    ASSERT_NE(detector, nullptr) << "Detector should initialize successfully";
}

// Test invalid paths
TEST_F(PerceptionTaskTest, InvalidPathTest) {
    EXPECT_THROW({
        HumanDetectorTracker invalid_detector("invalid.weights", "invalid.cfg", "invalid.names");
    }, cv::Exception) << "Should throw OpenCV exception for invalid files";
}

// Test basic frame processing
TEST_F(PerceptionTaskTest, BasicFrameTest) {
    if (detector == nullptr) {
        GTEST_SKIP() << "Detector not initialized, skipping test";
    }

    cv::Mat frame(480, 640, CV_8UC3, cv::Scalar(255, 255, 255));
    EXPECT_NO_THROW({
        detector->detectAndTrack(frame);
    }) << "Should process an empty frame without crashing";
}
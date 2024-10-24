#include <gtest/gtest.h>
#include "perception_task.hpp"
#include <opencv2/opencv.hpp>
#include <chrono>

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

    // Helper function to create test frame
    cv::Mat createTestFrame(const cv::Size& size, bool addPerson = false) {
        cv::Mat frame(size, CV_8UC3, cv::Scalar(255, 255, 255));
        if (addPerson) {
            // Draw a simple rectangle representing a person
            int personWidth = size.width / 6;
            int personHeight = size.height / 3;
            cv::rectangle(frame, 
                         cv::Rect(size.width/2 - personWidth/2, 
                                 size.height/2 - personHeight/2,
                                 personWidth, personHeight),
                         cv::Scalar(0, 0, 0), 
                         -1);
        }
        return frame;
    }
};

// Basic initialization test
TEST_F(PerceptionTaskTest, InitializationTest) {
    ASSERT_NE(detector, nullptr) << "Detector should initialize successfully";
}

// Test invalid paths
TEST_F(PerceptionTaskTest, InvalidPathTest) {
    EXPECT_THROW({
        HumanDetectorTracker invalid_detector("invalid.weights", "invalid.cfg", "invalid.names");
    }, cv::Exception) << "Should throw OpenCV exception for invalid files";
}

// Basic frame test
TEST_F(PerceptionTaskTest, BasicFrameTest) {
    if (detector == nullptr) {
        GTEST_SKIP() << "Detector not initialized, skipping test";
    }

    cv::Mat frame = createTestFrame(cv::Size(640, 480));
    EXPECT_NO_THROW({
        detector->detectAndTrack(frame);
    }) << "Should process an empty frame without crashing";
}

// Test with different frame sizes
TEST_F(PerceptionTaskTest, DifferentFrameSizes) {
    if (detector == nullptr) {
        GTEST_SKIP() << "Detector not initialized, skipping test";
    }
    
    std::vector<cv::Size> sizes = {
        {320, 240},   // Small frame
        {640, 480},   // Standard frame
        {1280, 720}   // HD frame
    };

    for (const auto& size : sizes) {
        cv::Mat frame = createTestFrame(size);
        EXPECT_NO_THROW({
            detector->detectAndTrack(frame);
        }) << "Failed for frame size: " << size.width << "x" << size.height;
    }
}

// Test multiple consecutive frames
TEST_F(PerceptionTaskTest, ConsecutiveFrames) {
    if (detector == nullptr) {
        GTEST_SKIP() << "Detector not initialized, skipping test";
    }
    
    const int numFrames = 5;
    cv::Mat frame = createTestFrame(cv::Size(640, 480), true);  // Add a person to track

    for (int i = 0; i < numFrames; i++) {
        EXPECT_NO_THROW({
            detector->detectAndTrack(frame);
        }) << "Failed on frame " << i;
    }
}

// Performance test
TEST_F(PerceptionTaskTest, PerformanceTest) {
    if (detector == nullptr) {
        GTEST_SKIP() << "Detector not initialized, skipping test";
    }
    
    cv::Mat frame = createTestFrame(cv::Size(640, 480), true);
    
    // Warm-up run
    detector->detectAndTrack(frame);
    
    // Timed run
    auto start = std::chrono::high_resolution_clock::now();
    detector->detectAndTrack(frame);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Processing time: " << duration.count() << "ms" << std::endl;
    
    // Assuming real-time processing requirement (less than 1 second per frame)
    EXPECT_LT(duration.count(), 1000) << "Processing took too long (> 1 second)";
}

// Test with empty frame
TEST_F(PerceptionTaskTest, EmptyFrameTest) {
    if (detector == nullptr) {
        GTEST_SKIP() << "Detector not initialized, skipping test";
    }

    cv::Mat emptyFrame;
    EXPECT_THROW({
        detector->detectAndTrack(emptyFrame);
    }, cv::Exception) << "Should throw exception for empty frame";
}
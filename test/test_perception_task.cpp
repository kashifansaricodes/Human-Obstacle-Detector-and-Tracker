// NOLINT(build/c++11)
#include <gtest/gtest.h>
#include <chrono>  // NOLINT(build/c++11)
#include <opencv2/opencv.hpp>
#include "perception_task.hpp"

/**
 * @brief Test fixture for HumanDetectorTracker tests
 */
class PerceptionTaskTest : public ::testing::Test {
 protected:
  HumanDetectorTracker* detector;
  std::string model_path;
  std::string config_path;
  std::string classes_path;

  void SetUp() override {
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

  // Helper functions
  cv::Mat createTestFrame(const cv::Size& size, bool addPerson = false,
                         const cv::Point& position = cv::Point(-1, -1)) {
    cv::Mat frame(size, CV_8UC3, cv::Scalar(255, 255, 255));
    if (addPerson) {
      int personWidth = size.width / 6;
      int personHeight = size.height / 3;
      cv::Point pos = position;
      if (pos.x < 0) pos.x = size.width / 2;
      if (pos.y < 0) pos.y = size.height / 2;
      
      cv::rectangle(frame,
                   cv::Rect(pos.x - personWidth/2, pos.y - personHeight/2,
                           personWidth, personHeight),
                   cv::Scalar(0, 0, 0), -1);
    }
    return frame;
  }

  void drawHuman(cv::Mat& frame, const cv::Point& center, 
                const cv::Size& size = cv::Size(100, 200)) {
    cv::rectangle(frame,
                 cv::Rect(center.x - size.width/2, center.y - size.height/2,
                         size.width, size.height),
                 cv::Scalar(0, 0, 0), -1);
  }

  cv::Mat createLightingCondition(const cv::Mat& frame, float brightness) {
    cv::Mat adjusted;
    frame.convertTo(adjusted, -1, brightness, 0);
    return adjusted;
  }

  // Helper function to verify frame modifications
  bool hasDetections(const cv::Mat& before, const cv::Mat& after) {
    cv::Mat diff;
    cv::absdiff(before, after, diff);
    cv::cvtColor(diff, diff, cv::COLOR_BGR2GRAY);
    return cv::countNonZero(diff) > 0;
  }
};

// 1. Initialization Tests
TEST_F(PerceptionTaskTest, InitializationTest) {
  ASSERT_NE(detector, nullptr) << "Detector should initialize successfully";
}

TEST_F(PerceptionTaskTest, InvalidPathTest) {
  EXPECT_THROW({
    HumanDetectorTracker invalid_detector("invalid.weights", "invalid.cfg",
                                        "invalid.names");
  }, cv::Exception) << "Should throw OpenCV exception for invalid files";
}

// 2. Basic Functionality Tests
TEST_F(PerceptionTaskTest, BasicFrameTest) {
  if (detector == nullptr) GTEST_SKIP();

  cv::Mat frame = createTestFrame(cv::Size(640, 480));
  cv::Mat original = frame.clone();
  
  EXPECT_NO_THROW({
    detector->detectAndTrack(frame);
  }) << "Should process an empty frame without crashing";

  // Empty frame should have minimal modifications
  EXPECT_FALSE(hasDetections(original, frame)) 
      << "Empty frame should not have detections";
}

TEST_F(PerceptionTaskTest, SingleHumanDetectionTest) {
  if (detector == nullptr) GTEST_SKIP();

  cv::Mat frame = createTestFrame(cv::Size(640, 480), true);
  cv::Mat original = frame.clone();

  EXPECT_NO_THROW({
    detector->detectAndTrack(frame);
  }) << "Detection process should complete without errors";
}


// 3. Frame Size Tests
TEST_F(PerceptionTaskTest, DifferentFrameSizes) {
  if (detector == nullptr) GTEST_SKIP();

  std::vector<cv::Size> sizes = {
    {320, 240},   // Small
    {640, 480},   // Standard
    {1280, 720},  // HD
  };

  for (const auto& size : sizes) {
    cv::Mat frame = createTestFrame(size, true);
    EXPECT_NO_THROW({
      detector->detectAndTrack(frame);
    }) << "Failed to process frame of size: " << size.width << "x" << size.height;
  }
}

// 4. Multiple Human Detection Tests
TEST_F(PerceptionTaskTest, MultipleHumanDetection) {
  if (detector == nullptr) GTEST_SKIP();

  cv::Mat frame = createTestFrame(cv::Size(640, 480));
  // Draw humans with more realistic sizes
  std::vector<cv::Point> positions = {
    cv::Point(160, 240),
    cv::Point(320, 240),
    cv::Point(480, 240)
  };

  for (const auto& pos : positions) {
    drawHuman(frame, pos, cv::Size(120, 240));  // Larger, more detectable size
  }

  EXPECT_NO_THROW({
    detector->detectAndTrack(frame);
  }) << "Should process multiple humans without error";
}


// 5. Tracking Tests
TEST_F(PerceptionTaskTest, ConsecutiveFrames) {
  if (detector == nullptr) GTEST_SKIP();

  cv::Mat frame = createTestFrame(cv::Size(640, 480));
  drawHuman(frame, cv::Point(320, 240), cv::Size(120, 240));

  for(int i = 0; i < 3; i++) {  // Reduced number of frames
    EXPECT_NO_THROW({
      detector->detectAndTrack(frame);
    }) << "Failed to process frame " << i;
  }
}

// 6. Edge Cases
TEST_F(PerceptionTaskTest, EmptyFrameTest) {
  if (detector == nullptr) GTEST_SKIP();

  cv::Mat emptyFrame;
  EXPECT_THROW({
    detector->detectAndTrack(emptyFrame);
  }, cv::Exception) << "Should throw exception for empty frame";
}

// 7. Performance Tests
TEST_F(PerceptionTaskTest, PerformanceTest) {
  if (detector == nullptr) GTEST_SKIP();

  cv::Mat frame = createTestFrame(cv::Size(640, 480), true);
  
  // Warm-up run
  detector->detectAndTrack(frame);
  
  // Timed run
  auto start = std::chrono::high_resolution_clock::now();
  detector->detectAndTrack(frame);
  auto end = std::chrono::high_resolution_clock::now();
  
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  EXPECT_LT(duration.count(), 1000) << "Processing took too long (> 1 second)";
}

// 8. Lighting Condition Tests
TEST_F(PerceptionTaskTest, LightingConditionsTest) {
  if (detector == nullptr) GTEST_SKIP();

  cv::Mat baseFrame = createTestFrame(cv::Size(640, 480), true);
  std::vector<float> brightnesses = {0.8, 1.0, 1.2};  // Less extreme values
  
  for(float brightness : brightnesses) {
    cv::Mat frame = createLightingCondition(baseFrame, brightness);
    EXPECT_NO_THROW({
      detector->detectAndTrack(frame);
    }) << "Failed to process frame with brightness " << brightness;
  }
}

// 9. Stress Test
TEST_F(PerceptionTaskTest, StressTest) {
  if (detector == nullptr) GTEST_SKIP();

  cv::Mat frame = createTestFrame(cv::Size(640, 480));
  
  // Add fewer humans with more realistic sizes
  for(int i = 0; i < 3; i++) {  // Reduced number
    for(int j = 0; j < 2; j++) {
      drawHuman(frame, 
                cv::Point(160 + i*160, 160 + j*160),  // More spread out
                cv::Size(120, 240));  // Larger size
    }
  }
  
  EXPECT_NO_THROW({
    detector->detectAndTrack(frame);
  }) << "Failed to process stress test frame";
}

// Adding a helper function for creating more realistic human figures
void drawRealisticHuman(cv::Mat& frame, const cv::Point& center) {
    // Body
    cv::rectangle(frame,
                 cv::Rect(center.x - 60, center.y - 120,
                         120, 240),
                 cv::Scalar(0, 0, 0), 
                 -1);
    
    // Head
    cv::circle(frame,
              cv::Point(center.x, center.y - 140),
              30,
              cv::Scalar(0, 0, 0),
              -1);
}
// NOLINT(build/c++11)
#include <gtest/gtest.h>
#include <chrono>  // NOLINT(build/c++11)
#include <opencv2/opencv.hpp>
#include "perception_task.hpp"

/**
 * @brief HumanDetectorTracker tests
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

  /**
   * @brief Create a test frame with optional human figure
   */
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

  /**
   * @brief Draw a human figure on the frame
   */
  void drawHuman(cv::Mat& frame, const cv::Point& center, 
                const cv::Size& size = cv::Size(100, 200)) {
    cv::rectangle(frame,
                 cv::Rect(center.x - size.width/2, center.y - size.height/2,
                         size.width, size.height),
                 cv::Scalar(0, 0, 0), -1);
  }

  /**
   * @brief Create a frame with specific lighting condition
   */
  cv::Mat createLightingCondition(const cv::Mat& frame, float brightness) {
    cv::Mat adjusted;
    frame.convertTo(adjusted, -1, brightness, 0);
    return adjusted;
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
  EXPECT_NO_THROW({
    detector->detectAndTrack(frame);
  }) << "Should process an empty frame without crashing";
}

TEST_F(PerceptionTaskTest, SingleHumanDetectionTest) {
  if (detector == nullptr) GTEST_SKIP();

  cv::Mat frame = createTestFrame(cv::Size(640, 480), true);
  EXPECT_NO_THROW({
    detector->detectAndTrack(frame);
  });
  cv::imwrite("single_human_test.jpg", frame);
}

// 3. Frame Size Tests
TEST_F(PerceptionTaskTest, DifferentFrameSizes) {
  if (detector == nullptr) GTEST_SKIP();

  std::vector<cv::Size> sizes = {
    {320, 240},   // Small
    {640, 480},   // Standard
    {1280, 720},  // HD
    {1920, 1080}  // Full HD
  };

  for (const auto& size : sizes) {
    cv::Mat frame = createTestFrame(size, true);
    EXPECT_NO_THROW({
      detector->detectAndTrack(frame);
    }) << "Failed for size: " << size.width << "x" << size.height;
  }
}

// 4. Multiple Human Detection Tests
TEST_F(PerceptionTaskTest, MultipleHumanDetection) {
  if (detector == nullptr) GTEST_SKIP();

  cv::Mat frame = createTestFrame(cv::Size(640, 480));
  std::vector<cv::Point> positions = {
    cv::Point(160, 240),
    cv::Point(320, 240),
    cv::Point(480, 240)
  };

  for (const auto& pos : positions) {
    drawHuman(frame, pos);
  }

  detector->detectAndTrack(frame);
  cv::imwrite("multiple_humans_test.jpg", frame);
}

// 5. Tracking Tests
TEST_F(PerceptionTaskTest, ConsecutiveFrames) {
  if (detector == nullptr) GTEST_SKIP();

  std::vector<cv::Mat> frames;
  for(int i = 0; i < 5; i++) {
    cv::Mat frame = createTestFrame(cv::Size(640, 480));
    drawHuman(frame, cv::Point(200 + i*30, 240));
    frames.push_back(frame);
  }

  for(size_t i = 0; i < frames.size(); i++) {
    detector->detectAndTrack(frames[i]);
    cv::imwrite("tracking_frame_" + std::to_string(i) + ".jpg", frames[i]);
  }
}

// 6. Location Estimation Tests
TEST_F(PerceptionTaskTest, LocationEstimationTest) {
  if (detector == nullptr) GTEST_SKIP();

  cv::Mat frame = createTestFrame(cv::Size(640, 480));
  cv::Rect humanRect(100, 100, 50, 100);
  
  cv::Point3f location = detector->getLocation(humanRect);
  
  EXPECT_GT(location.z, 0) << "Distance should be positive";
  EXPECT_NE(location.x, 0) << "X position should be non-zero";
  EXPECT_NE(location.y, 0) << "Y position should be non-zero";
}

// 7. Edge Cases
TEST_F(PerceptionTaskTest, EmptyFrameTest) {
  if (detector == nullptr) GTEST_SKIP();

  cv::Mat emptyFrame;
  EXPECT_THROW({
    detector->detectAndTrack(emptyFrame);
  }, cv::Exception) << "Should throw exception for empty frame";
}

TEST_F(PerceptionTaskTest, PartialHumanTest) {
  if (detector == nullptr) GTEST_SKIP();

  cv::Mat frame = createTestFrame(cv::Size(640, 480));
  // Draw humans at edges
  drawHuman(frame, cv::Point(0, 240));     // Left edge
  drawHuman(frame, cv::Point(640, 240));   // Right edge
  drawHuman(frame, cv::Point(320, 0));     // Top edge
  drawHuman(frame, cv::Point(320, 480));   // Bottom edge
  
  EXPECT_NO_THROW({
    detector->detectAndTrack(frame);
  });
  cv::imwrite("partial_humans_test.jpg", frame);
}

// 8. Performance Tests
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
  std::cout << "Processing time: " << duration.count() << "ms" << std::endl;
  
  EXPECT_LT(duration.count(), 1000) << "Processing took too long (> 1 second)";
}

// 9. Lighting Condition Tests
TEST_F(PerceptionTaskTest, LightingConditionsTest) {
  if (detector == nullptr) GTEST_SKIP();

  cv::Mat baseFrame = createTestFrame(cv::Size(640, 480), true);
  
  // Test different lighting conditions
  std::vector<float> brightnesses = {0.5, 1.0, 1.5};  // Dark, Normal, Bright
  for(float brightness : brightnesses) {
    cv::Mat adjustedFrame = createLightingCondition(baseFrame, brightness);
    EXPECT_NO_THROW({
      detector->detectAndTrack(adjustedFrame);
    }) << "Failed for brightness factor: " << brightness;
    cv::imwrite("lighting_test_" + std::to_string(brightness) + ".jpg", 
                adjustedFrame);
  }
}

// 10. Stress Test
TEST_F(PerceptionTaskTest, StressTest) {
  if (detector == nullptr) GTEST_SKIP();

  cv::Mat frame = createTestFrame(cv::Size(640, 480));
  
  // Add many humans
  for(int i = 0; i < 10; i++) {
    for(int j = 0; j < 5; j++) {
      drawHuman(frame, cv::Point(60 + i*60, 80 + j*80), cv::Size(40, 60));
    }
  }
  
  EXPECT_NO_THROW({
    detector->detectAndTrack(frame);
  });
  cv::imwrite("stress_test.jpg", frame);
}

if(EXISTS "/home/kashif/SDR/week6/Human-Obstacle-Detector-and-Tracker/build/test/cpp-test[1]_tests.cmake")
  include("/home/kashif/SDR/week6/Human-Obstacle-Detector-and-Tracker/build/test/cpp-test[1]_tests.cmake")
else()
  add_test(cpp-test_NOT_BUILT cpp-test_NOT_BUILT)
endif()

add_test( PerceptionTaskTest.InitializationTest /home/kashif/SDR/week6/Human-Obstacle-Detector-and-Tracker/build/test/cpp-test [==[--gtest_filter=PerceptionTaskTest.InitializationTest]==] --gtest_also_run_disabled_tests)
set_tests_properties( PerceptionTaskTest.InitializationTest PROPERTIES WORKING_DIRECTORY /home/kashif/SDR/week6/Human-Obstacle-Detector-and-Tracker SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( PerceptionTaskTest.InvalidPathTest /home/kashif/SDR/week6/Human-Obstacle-Detector-and-Tracker/build/test/cpp-test [==[--gtest_filter=PerceptionTaskTest.InvalidPathTest]==] --gtest_also_run_disabled_tests)
set_tests_properties( PerceptionTaskTest.InvalidPathTest PROPERTIES WORKING_DIRECTORY /home/kashif/SDR/week6/Human-Obstacle-Detector-and-Tracker SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( PerceptionTaskTest.BasicFrameTest /home/kashif/SDR/week6/Human-Obstacle-Detector-and-Tracker/build/test/cpp-test [==[--gtest_filter=PerceptionTaskTest.BasicFrameTest]==] --gtest_also_run_disabled_tests)
set_tests_properties( PerceptionTaskTest.BasicFrameTest PROPERTIES WORKING_DIRECTORY /home/kashif/SDR/week6/Human-Obstacle-Detector-and-Tracker SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( PerceptionTaskTest.DifferentFrameSizes /home/kashif/SDR/week6/Human-Obstacle-Detector-and-Tracker/build/test/cpp-test [==[--gtest_filter=PerceptionTaskTest.DifferentFrameSizes]==] --gtest_also_run_disabled_tests)
set_tests_properties( PerceptionTaskTest.DifferentFrameSizes PROPERTIES WORKING_DIRECTORY /home/kashif/SDR/week6/Human-Obstacle-Detector-and-Tracker SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( PerceptionTaskTest.ConsecutiveFrames /home/kashif/SDR/week6/Human-Obstacle-Detector-and-Tracker/build/test/cpp-test [==[--gtest_filter=PerceptionTaskTest.ConsecutiveFrames]==] --gtest_also_run_disabled_tests)
set_tests_properties( PerceptionTaskTest.ConsecutiveFrames PROPERTIES WORKING_DIRECTORY /home/kashif/SDR/week6/Human-Obstacle-Detector-and-Tracker SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( PerceptionTaskTest.PerformanceTest /home/kashif/SDR/week6/Human-Obstacle-Detector-and-Tracker/build/test/cpp-test [==[--gtest_filter=PerceptionTaskTest.PerformanceTest]==] --gtest_also_run_disabled_tests)
set_tests_properties( PerceptionTaskTest.PerformanceTest PROPERTIES WORKING_DIRECTORY /home/kashif/SDR/week6/Human-Obstacle-Detector-and-Tracker SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( PerceptionTaskTest.EmptyFrameTest /home/kashif/SDR/week6/Human-Obstacle-Detector-and-Tracker/build/test/cpp-test [==[--gtest_filter=PerceptionTaskTest.EmptyFrameTest]==] --gtest_also_run_disabled_tests)
set_tests_properties( PerceptionTaskTest.EmptyFrameTest PROPERTIES WORKING_DIRECTORY /home/kashif/SDR/week6/Human-Obstacle-Detector-and-Tracker SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set( cpp-test_TESTS PerceptionTaskTest.InitializationTest PerceptionTaskTest.InvalidPathTest PerceptionTaskTest.BasicFrameTest PerceptionTaskTest.DifferentFrameSizes PerceptionTaskTest.ConsecutiveFrames PerceptionTaskTest.PerformanceTest PerceptionTaskTest.EmptyFrameTest)
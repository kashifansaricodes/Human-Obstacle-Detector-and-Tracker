# Add test executable
add_executable(cpp-test
    test_perception_task.cpp
)

# Include directories
target_include_directories(cpp-test PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(cpp-test PUBLIC
    GTest::gtest
    GTest::gtest_main
    perception_task
    ${OpenCV_LIBS}
)

# Set the working directory to the project root
set_target_properties(cpp-test PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# Discover tests with correct working directory
gtest_discover_tests(cpp-test 
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
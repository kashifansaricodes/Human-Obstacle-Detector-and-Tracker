# Add the executable
add_executable(shell-app
  main.cpp
)

# Include directories
target_include_directories(shell-app PUBLIC
  ${CMAKE_SOURCE_DIR}/include
  ${OpenCV_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(shell-app PUBLIC
  perception_task
  ${OpenCV_LIBS}
)
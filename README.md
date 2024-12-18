# Human Obstacle Detection and Tracking

![CICD Workflow status](https://github.com/kashifansaricodes/Human-Obstacle-Detector-and-Tracker/actions/workflows/run-unit-test-and-upload-codecov.yml/badge.svg) [![codecov](https://codecov.io/gh/kashifansaricodes/Human-Obstacle-Detector-and-Tracker/graph/badge.svg?token=T484S8WKBC)](https://codecov.io/gh/kashifansaricodes/Human-Obstacle-Detector-and-Tracker)[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

The Human Obstacle Detection and Tracking software is a perception stack for robots to navigate robustly around humans. The robots are equipped with a monocular camera with the help of which it identifies and tracks humans. This enables the robot to be around humans in a safe manner. 

The code in this repository was contributed by Kashif Ansari, Abhishek Avadh and Piyush Goenka, all of which are pursuing their M.Eng. in Robotics degree from the University of Maryland. This repository was developed for the Mid-term project of the  course ENPM700: Software Development for Robotics. 

The software uses an RGB camera attached to the robot as input. The use of YOLO and OpenCV enables the processing of the image. The below screenshot conveys the output of the software. 

![Screenshot from 2024-10-30 20-53-23](https://github.com/user-attachments/assets/4a30789d-3e4a-4f13-bf37-cdfd9cae9016)


### Documents and Links

1. **Documents:** The Quadcharts, Proposals and Software Plan can be found in the sub-directory *Documents*

2. **UML:** The UML diagrams can be found in the sub-directory *UML*

3. **Sprint sheet:** This [*Sprint Sheet*](https://docs.google.com/document/d/1m02yIOxLIv32R1kmjg583Tf8zdp8dUxPbcdExJ11MYU/edit?usp=sharing) provides a detailed overview of our project’s sprint planning and progress tracking. This document helps ensure transparency, accountability, and consistent progress toward project goals.

4. **Backlog link:** https://docs.google.com/spreadsheets/d/1nO3FHj5wFkExcySgvAdLF-TEKWzpPiGhxI9WFZUwfwI/edit?gid=0#gid=0

5. **Proposal Video Link:** https://youtu.be/3WHn_zyDtvE?si=f9rTs8p5hs1TCHXb

6. **Final Video and Demo:**

[![Demo Video](https://github.com/kashifansaricodes/Human-Obstacle-Detector-and-Tracker/blob/master/demo.png)](https://www.youtube.com/watch?v=VOHsfNJf0cY)

### Dependencies

This project uses the OpenCV C++ library for computer vision tasks. Our system also uses a YOLO object detection model. This model helps our system to detect humans in the camera frame. These libraries are central to the working of our system. OpenCV deals with the fundamental image operations such as resizing, reshaping, formatting, etc. YOLO deals with human detection and tracking.

### Installing Dependencies

```bash
# install OpenCV
sudo apt install libopencv-dev
```

### Download and Build the workspace
```bash
# Download the code:
  git clone https://github.com/kashifansaricodes/Human-Obstacle-Detector-and-Tracker
  cd Human-Obstacle-Detector-and-Tracker

# Configure the project and generate a native build system:
  # Must re-run this command whenever any CMakeLists.txt file has been changed.
  cmake -S ./ -B build/

# Compile and build the project:
  # rebuild only files that are modified since the last build
  cmake --build build/
  # or rebuild everything from scracth
  cmake --build build/ --clean-first
  # to see verbose output, do:
  cmake --build build/ --verbose
```

### Generate Documentation

To generate and view the Doxygen documentation of the project, run the following commands.

```bash
# Build docs:
  cmake --build build/ --target docs
  # open a web browser to browse the doc
  open docs/html/index.html
```

### Run the code 

```bash
# Run program:
  ./build/app/shell-app
```
### Run Unit tests

To manually run and verify the working of the unit tests, execute the commands below.

```bash
# Run tests:
  cd build/; ctest; cd -
  # or if you have newer cmake
  ctest --test-dir build/
```

### Building for Test Coverage

```bash
# If you don't have gcovr or lcov installed, run:
  sudo apt-get install gcovr lcov
# Set the build type to Debug and WANT_COVERAGE=ON:
  cmake -D WANT_COVERAGE=ON -D CMAKE_BUILD_TYPE=Debug -S ./ -B build/
# Do a clean compile, run unit test, and generate the coverage report:
  cmake --build build/ --clean-first --target all test_coverage
# Open a web browser to browse the test coverage report:
  open build/test_coverage/index.html
```

### Google Coding Style Verification
```bash
# Install Cpplint(ignore if already installed):
  sudo apt install cpplint
# Self-check Google code style conformity using Cpplint:
  cpplint --filter="-legal/copyright" $( find . -name *.cpp | grep -vE -e "^./build/" )
```

### Static Code Analysis
```bash
# Install Cppcheck (ignore if already installed):
  sudo apt install cppcheck
# Self-check the static code analysis using Cppcheck:
  cppcheck --enable=all --std=c++11 --suppress=missingInclude --suppress=unusedFunction $( find . -name *.cpp | grep -vE -e "^./build/" )
```

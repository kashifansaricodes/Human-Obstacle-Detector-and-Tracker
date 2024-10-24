# Human Obstacle Detection and Tracking

![CICD Workflow status](https://github.com/kashifansaricodes/Human-Obstacle-Detector-and-Tracker/blob/master/.github/workflows/run-unit-test-and-upload-codecov.yml/badge.svg) [![codecov](https://codecov.io/gh/TommyChangUMD/cpp-boilerplate-v2/branch/main/graph/badge.svg)](https://codecov.io/gh/TommyChangUMD/cpp-boilerplate-v2) [![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

## For QuadCharts, Proposals and Software Plan refer sub-directory Documents
## For UML diagrams, refer to sub-directory UML

## Proposal Video Link: https://youtu.be/3WHn_zyDtvE?si=f9rTs8p5hs1TCHXb

## Backlog link: https://docs.google.com/spreadsheets/d/1nO3FHj5wFkExcySgvAdLF-TEKWzpPiGhxI9WFZUwfwI/edit?gid=0#gid=0

## Standard protocol to run the workspace
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
# Run program:
  ./build/app/shell-app
# Run tests:
  cd build/; ctest; cd -
  # or if you have newer cmake
  ctest --test-dir build/
# Build docs:
  cmake --build build/ --target docs
  # open a web browser to browse the doc
  open docs/html/index.html

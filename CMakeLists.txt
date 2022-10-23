cmake_minimum_required(VERSION 3.14)
project(ruuvi_parser_lib)

set(CMAKE_CXX_STANDARD 14)

SET (PROJECT_ROOT "${PROJECT_SOURCE_DIR}")
SET (PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  RuuviMeasurementParserTest
  ${PROJECT_SOURCE_DIR}/RuuviMeasurementParserTest.cpp
  ${PROJECT_SOURCE_DIR}/RuuviMeasurementParser.cpp
)
target_link_libraries(
  RuuviMeasurementParserTest
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(RuuviMeasurementParserTest)

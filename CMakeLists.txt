cmake_minimum_required(VERSION 3.6)
project(cv_data_test)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 11)

include_directories(${OpenCV_INCLUDE_DIRS})

set(SOURCE_FILES src/main.cpp)
add_executable(cv_data_test ${SOURCE_FILES})

target_link_libraries(cv_data_test ${OpenCV_LIBS})
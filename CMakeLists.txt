cmake_minimum_required(VERSION 3.6)
project(cv_data_test)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 11)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(ocv_test src/openCvTest.cpp)
target_link_libraries(ocv_test ${OpenCV_LIBS})

add_executable(inria_demo src/inriaHeadPose.cpp)
target_link_libraries(inria_demo ${OpenCV_LIBS})

add_executable(cmu_demo src/cmuFaces.cpp)
target_link_libraries(cmu_demo ${OpenCV_LIBS})

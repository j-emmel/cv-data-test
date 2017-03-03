cmake_minimum_required(VERSION 3.6)
project(cv_data_test)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)
find_package(Boost COMPONENTS REQUIRED system filesystem iostreams)

add_subdirectory(demo-support "${CMAKE_CURRENT_BINARY_DIR}/comp-viz-helpers")

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories (demo-support/include)

add_executable(ocv_test src/openCvTest.cpp)
target_link_libraries(ocv_test ${OpenCV_LIBS})

add_executable(inria_demo src/inriaHeadPose.cpp)
target_link_libraries(inria_demo ${OpenCV_LIBS})

add_executable(cmu_demo src/cmuFaces.cpp)
target_link_libraries(cmu_demo ${OpenCV_LIBS})
target_link_libraries (cmu_demo demo-support)

add_executable(ncku_demo src/nckuSet.cpp)
target_link_libraries(ncku_demo ${OpenCV_LIBS})
target_link_libraries (ncku_demo demo-support)

add_executable(feret_demo src/feretDb.cpp)
target_link_libraries(feret_demo ${OpenCV_LIBS} ${Boost_LIBRARIES})


# http://stackoverflow.com/questions/13429656
add_custom_command(TARGET cmu_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:cmu_demo>)
add_custom_command(TARGET ncku_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:ncku_demo>)
add_custom_command(TARGET feret_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:feret_demo>)

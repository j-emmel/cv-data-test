find_package(OpenCV REQUIRED)

include_directories(include)

add_library(comp-viz-helpers
        include/demo-support.h
        src/SimpleFaceDetector.cpp)
target_link_libraries(comp-viz-helpers ${OpenCV_LIBS})
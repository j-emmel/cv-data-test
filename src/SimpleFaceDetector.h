#ifndef CV_DATA_TEST_SIMPLEFACEDETECTOR_H
#define CV_DATA_TEST_SIMPLEFACEDETECTOR_H

#include <opencv2/opencv.hpp>


using std::cout;
using std::cerr;
using std::endl;
using std::string;

class SimpleFaceDetector {
private:
    cv::CascadeClassifier faceCascade;
    cv::CascadeClassifier eyesCascade;
public:
    SimpleFaceDetector(const cv::CascadeClassifier &faceCascade, const cv::CascadeClassifier &eyesCascade);
    cv::Mat detectAndOverlay(const cv::Mat image);
};


#endif

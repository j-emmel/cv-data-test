#ifndef CV_DATA_TEST_SIMPLEFACEDETECTOR_H
#define CV_DATA_TEST_SIMPLEFACEDETECTOR_H

#include <opencv2/core/mat.hpp>
#include <opencv2/objdetect.hpp>

class SimpleFaceDetector {
private:
    cv::CascadeClassifier faceCascade;
    cv::CascadeClassifier eyesCascade;
public:
    SimpleFaceDetector(const cv::CascadeClassifier &faceCascade, const cv::CascadeClassifier &eyesCascade);
    SimpleFaceDetector(const std::string &faceCascadePath, const std::string &eyesCasdadePath);
    cv::Mat detectAndOverlay(cv::Mat image);
};


#endif

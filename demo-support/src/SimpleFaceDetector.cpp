#include <opencv2/opencv.hpp>

#include "demo-support.h"

SimpleFaceDetector::SimpleFaceDetector(const cv::CascadeClassifier &faceCascade,
                                       const cv::CascadeClassifier &eyesCascade)
        : faceCascade(faceCascade), eyesCascade(eyesCascade) {}

SimpleFaceDetector::SimpleFaceDetector(const std::string &faceCascadePath, const std::string &eyesCascadePath)
        : faceCascade(cv::String(faceCascadePath)), eyesCascade(cv::String(eyesCascadePath)) {}


/**
 * Attempts to locate faces and eyes in an input image.
 * @param image a BGR image to run through face detection
 * @return the input image with any face or eye detections marked by rectangles
 */
cv::Mat SimpleFaceDetector::detectAndOverlay(const cv::Mat image) {
    cv::Mat imageGrayscale(image);
    cv::cvtColor(image, imageGrayscale, CV_BGR2GRAY);
    cv::equalizeHist(imageGrayscale, imageGrayscale);

    // Detect and iterate over faces
    std::vector<cv::Rect> faces;
    faceCascade.detectMultiScale(imageGrayscale, faces, 1.1, 2);
    for (cv::Rect face : faces) {
        cv::rectangle(
            image,
            cv::Point(face.x, face.y),
            cv::Point(face.x + face.width, face.y + face.height),
            CV_RGB(255, 0, 0),
            2
        );

        // Detect and iterate over eyes
        std::vector<cv::Rect> eyes;
        eyesCascade.detectMultiScale(imageGrayscale(face), eyes, 1.1, 2);
        for (cv::Rect eye : eyes) {
            int eyeX = face.x + eye.x;
            int eyeY = face.y + eye.y;

            cv::rectangle(
                image,
                cv::Point(eyeX, eyeY),
                cv::Point(eyeX + eye.width, eyeY + eye.height),
                CV_RGB(0, 255, 0),
                2
            );
        }
    }

    return image;
}

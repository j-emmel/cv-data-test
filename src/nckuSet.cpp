/* ***
 * Iterates through the data set of NCKU's robotics lab.
 *
 * File names are formatted Subject[##]/A_[##]_[Pose].jpg
 * 'A' above signifies a real photograph. 'B' is used for synthesized images.
 *
 * Data Set:
 *   http://robotics.csie.ncku.edu.tw/Databases/FaceDetect_PoseEstimate.htm#Our_Database_
 *
 * Data Set Licensing:
 *   No information given
 * ***/

#include <string>
#include <iostream>

#include <opencv2/opencv.hpp>
#include "demo-support.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

bool process(SimpleFaceDetector& faceDetector, const string dir, const char type, const int subjectNum, int pose) {
    string imgFilePath;
    cv::Mat image;
    cv::Mat imageOverlaid;

    string poseSign;
    int posePad = 2;

    if (pose < 0) {
        poseSign = "-";
        pose *= -1;
    } else if (pose > 0) {
        poseSign = "+";
    } else {
        poseSign = "";
        posePad = 0;
    }

    std::ostringstream fileNameStream;
    fileNameStream << dir << "Subject"
                   << std::setfill('0') << std::setw(2) << subjectNum << "/"
                   << type << "_"
                   << std::setw(2) << subjectNum << std::setw(0) << "_"
                   << poseSign
                   << std::setw(posePad) << pose << ".Jpg";
    imgFilePath = fileNameStream.str();


    image = cv::imread(imgFilePath, cv::IMREAD_COLOR);
    if (image.data == NULL) {
        cerr << "Could not load image " << imgFilePath << endl;
        return false;
    }

    cout << "Loaded " << imgFilePath << endl;
    imageOverlaid = faceDetector.detectAndOverlay(image);
    cv::imshow("Face Detections", imageOverlaid);
    return true;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: ./ncku_demo PATH" << endl;
        cerr << "PATH: Path to the CMU data set (with ending /)" << endl;
        return EXIT_FAILURE;
    }

    string imgDir = argv[1];
    const std::string faceCascadePath = "haarcascade_frontalface_alt.xml";
    const std::string eyesCascadePath = "haarcascade_eye_tree_eyeglasses.xml";

    // Loop through the data set, running valid images through a face detector
    SimpleFaceDetector detector(faceCascadePath, eyesCascadePath);
    for (int i = 1; i <= 90; i++) {
        for (int pose = -90; pose <= 90; pose += 5) {
            process(detector, imgDir, 'A', i, pose);
            cv::waitKey(100);
        }
    }

    cv::destroyWindow("Face Detections");
    return EXIT_SUCCESS;
}

/* ***
 * Iterates through the CMU Face Images data set and runs OpenCV HAAR Cascade detectors on the images.
 *
 * File names are formatted [User ID]/[User ID]_[Pose]_[Expression]_[Eyes]_[Scale].pgm
 * _[Scale] is omitted for scale=1
 *
 * Data Set:
 *   https://archive.ics.uci.edu/ml/datasets/CMU+Face+Images
 *   https://archive.ics.uci.edu/ml/machine-learning-databases/faces-mld/faces.data.html
 *
 * Data Set Licensing:
 *   Usable for any educational purpose with attribution
 * ***/

#include <string>
#include <iostream>

#include <opencv2/opencv.hpp>
#include "SimpleFaceDetector.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

bool process(SimpleFaceDetector& faceDetector, const string dir, const string userId, const string pose,
             const string expression, const string eye, const int scale) {
    cv::Mat imageOverlaid;

    const string scaleStr = scale == 0 ? "" : ("_" + std::to_string(scale));
    const string imgFilePath = dir + userId + "/" + userId + "_" + pose + "_" + expression + "_" + eye + scaleStr + ".pgm";

    cv::Mat image = cv::imread(imgFilePath, cv::IMREAD_COLOR);
    if (image.data == NULL) {
        cerr << "Could not load image " << imgFilePath << " (may be bad?)" << endl;
        return false;
    }

    cout << "Loaded " << imgFilePath << endl;
    imageOverlaid = faceDetector.detectAndOverlay(image);
    cv::imshow("Face Detections", imageOverlaid);
    return true;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: ./cmu_demo PATH" << endl;
        cerr << "PATH: Path to the CMU data set (with ending /)" << endl;
        return EXIT_FAILURE;
    }

    const std::string faceCascadePath = "haarcascade_frontalface_alt.xml";
    const std::string eyesCascadePath = "haarcascade_eye_tree_eyeglasses.xml";

    // User IDs identify the twenty people whose images comprise the data set
    const std::vector<string> userIds = {
            "an2i", "at33", "boland", "bpm", "ch4f", "cheyer", "choon", "danieln", "glickman", "karyadi",
            "kawamura", "kk49", "megak", "mitchell", "night", "phoebe", "saavik", "steffi", "sz24", "tammo"
    };
    const std::vector<string> poses = { "straight", "left", "right", "up" };
    const std::vector<string> expressions = {  "neutral", "happy", "sad", "angry" };
    const std::vector<string> eyes = { "open", "sunglasses" };
    const std::vector<int> scales = { 0, 2, 4 };

    cv::Mat image;
    cv::CascadeClassifier faceCascade;
    cv::CascadeClassifier eyesCascade;

    string imgDir;
    string imgFilePath;

    imgDir = argv[1];

    if(!faceCascade.load(cv::String(faceCascadePath))) {
        cerr << "Error loading face cascade .xml file." << endl;
        return EXIT_FAILURE;
    };

    if(!eyesCascade.load(cv::String(eyesCascadePath))) {
        cerr << "Error loading eye cascade .xml file." << endl;
        return EXIT_FAILURE;
    };

    // Loop through the data set, running valid images through a face detector
    SimpleFaceDetector detector(faceCascade, eyesCascade);
    for (const string& userId : userIds) {
        for (const string& pose : poses) {
            for (const string& expression : expressions) {
                for (const string& eye : eyes) {
                    for (const int& scale : scales) {
                        process(detector, imgDir, userId, pose, expression, eye, scale);
                        cv::waitKey(300);
                    }
                }
            }
        }
    }

    cv::destroyWindow("Face Detections");
    return EXIT_SUCCESS;
}

/* ***
 * WIP: Iterates through the CMU Face Images data set.
 *
 * File names are formatted [User ID]/[User ID]_[Pose]_[Expression]_[Eyes]_[Scale].pgm
 * _[Scale] is omitted for scale=1
 *
 * Data Set:
 *   https://archive.ics.uci.edu/ml/datasets/CMU+Face+Images
 *   https://archive.ics.uci.edu/ml/machine-learning-databases/faces-mld/faces.data.html
 *
 * Licensing:
 *   Usable for any educational purpose with attribution
 * ***/

#include <string>
#include <iostream>

#include <opencv2/opencv.hpp>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

bool process(const string dir, const string userId, const string pose, const string expression, const string eye, const int scale) {
    string scaleStr = scale == 0 ? "" : ("_" + std::to_string(scale));
    string imgFilePath = dir + userId + "/" + userId + "_" + pose + "_" + expression + "_" + eye + scaleStr + ".pgm";

    cv::Mat image = cv::imread(imgFilePath, cv::IMREAD_UNCHANGED);
    if (image.data == NULL) {
        cerr << "Could not load image " << imgFilePath << " (may be bad?)" << endl;
        return false;
    }

    cout << "Loaded " << imgFilePath << endl;
    return true;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: ./cmu_demo PATH" << endl;
        cerr << "PATH: Path to the CMU data set (with ending /)" << endl;
        return EXIT_FAILURE;
    }

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
    string imgFilePath;
    string scaleStr;
    string imgDir;

    imgDir = argv[1];

    for (const string& userId : userIds) {
        for (const string& pose : poses) {
            for (const string& expression : expressions) {
                for (const string& eye : eyes) {
                    for (const int& scale : scales) {
                        process(imgDir, userId, pose, expression, eye, scale);
                    }
                }
            }
        }
    }

    return EXIT_SUCCESS;
}

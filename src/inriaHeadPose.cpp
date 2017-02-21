/* ***
 * Iterates through the images of INRIA's Head Pose Image Database, display each image overlaid
 * with its face's bounding box.
 *
 * File names are formatted person[ID][Series][Number][Tilt][Pan].jpg
 *
 * This code is based on the C browsing code provided on the data set's web page.
 *
 * Data Set:
 *   http://www-prima.inrialpes.fr/perso/Gourier/Faces/HPDatabase.html
 * Licensing:
 *   Usable for any purpose, as long as the source paper is cited:
 *   http://www-prima.inrialpes.fr/perso/Gourier/Pointing04-Gourier.pdf
 * ***/

#include <string>
#include <iostream>

#include <opencv2/opencv.hpp>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

int main(int argc, char** argv) {
    // Check number of arguments
    if (argc < 2) {
        cerr << "Usage: ./inria_demo PATH" << endl;
        cerr << "PATH: Path to the INRIA database directory (with ending /)" << endl;
        return EXIT_FAILURE;
    }

    const int personCount = 15;
    const int seriesCount = 2;
    const int photosPerSeries = 93;

    std::ostringstream fileNameStream;
    string imgDir;
    string imgFile;
    string txtFile;
    unsigned int xt1, yt1, xt2, yt2;

    // Label variables
    string name;
    string prop;
    unsigned int x, y, w, h;

    // Apply face detection on database images
    int pan = 0;
    int tilt = 0;
    int panIndex, tiltIndex;
    string panPlus;
    string tiltPlus;

    // Database installation directory
    imgDir = argv[1];

    // Create a window for displaying the faces
    cv::namedWindow("Source Image", cv::WINDOW_AUTOSIZE);
    cv::Mat image;

    for (int numPers = 1; numPers <= personCount; numPers++) {
        for (int numSer = 1; numSer <= seriesCount; numSer++) {
            for (int i = 0; i < photosPerSeries; i++) {
                // Retrieve pan and tilt angles
                if (i == 0) {
                    tilt = -90;
                    pan = 0;
                } else if (i == 92) {
                    tilt =  90;
                    pan = 0;
                } else {
                    pan  = ((i - 1) % 13 - 6) * 15;
                    tilt = ((i - 1) / 13 - 3) * 15;

                    if (abs(tilt) == 45) {
                        tilt = tilt / abs(tilt) * 60;
                    }
                }

                // Add "+" before positive angles
                panPlus = pan >= 0 ? "+" : "";
                tiltPlus = tilt >= 0 ? "+" : "";

                // Assemble the path to the image file
                fileNameStream.str("");
                fileNameStream << imgDir << "Person"
                              << std::setfill('0') << std::setw(2) << numPers << "/person"
                              << std::setw(2) << numPers
                              << std::setw(0) << numSer
                              << std::setw(2) << i
                              << tiltPlus << tilt << panPlus << pan << ".jpg";
                imgFile = fileNameStream.str();
                cout << "Processing " << imgFile << endl;

                image = cv::imread(imgFile, 1);
                if (image.data == NULL) {
                    cerr << "Could not load image " << imgFile << endl;
                    return EXIT_FAILURE;
                }

                // Note: image processing would take place here

                panIndex  = pan / 15 + 6;
                tiltIndex = (i + 12) / 13;

                // Assemble the name of the property file
                fileNameStream.str("");
                fileNameStream << imgDir << "Person"
                           << std::setfill('0') << std::setw(2) << numPers << "/person"
                           << std::setw(2) << numPers
                           << std::setw(0) << numSer
                           << std::setw(2) << i
                           << tiltPlus << tilt << panPlus << pan << ".txt";
                txtFile = fileNameStream.str();
                cout << "Opening file " << txtFile << endl;

                // Read in face location data from the property file
                std::ifstream propFile(txtFile);
                if (!propFile.is_open()) {
                    cerr << "Could not open file " << txtFile << endl;
                    continue;
                }
                propFile >> name >> prop >> x >> y >> w >> h;
                propFile.close();

                cout << prop << " at (" << x << "," << y << ") - (" << w << " x " << h << ")" << endl;

                // Corners of face's bounding box
                xt1 = x - w / 2;
                xt2 = x + w / 2;
                yt1 = y - h / 2;
                yt2 = y + h / 2;

                // Display rectangle
                cv::waitKey(30);
                cv::rectangle(image, cvPoint(xt1, yt1), cvPoint(xt2, yt2), CV_RGB(255, 0, 0), 2);
                cv::imshow("Source Image", image);
            }
        }
    }

    cv::destroyWindow("Source Image");
    return EXIT_SUCCESS;
}

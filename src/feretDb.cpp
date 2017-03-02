/* ***
 * Iterates through the Color FERET Database.
 *
 * File names are formatted colorferet/dvd[#]/data/images/[Subject ID]/[Subject ID]_[Capture Date]_[Pose].ppm.bz2
 *   dvd1: subjects 00001 - 00739
 *   dvd2: subjects 00740 - 01208
 *
 * Data Set:
 *   https://www.nist.gov/itl/iad/image-group/color-feret-database
 *
 * Data Set Licensing:
 *   https://www.nist.gov/sites/default/files/documents/2016/12/15/colorferet_release_agreement.pdf
 *   "available to researchers in face recognition on a case-by-case basis"
 * ***/

#include <string>
#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/copy.hpp>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

/**
 * Reads a Bzip2-compressed image file into an OpenCV matrix.
 * @param imgFilePath path to the compressed image file
 * @return A matrix containing the image data, or an empty matrix if the image could not be read
 */
cv::Mat imreadBz2(const string imgFilePath) {
    boost::iostreams::file_source bz2Input(imgFilePath, std::ios_base::in | std::ios_base::binary);

    if (!bz2Input.is_open()) {
        cout << "Invalid input: " << imgFilePath << endl;
        return cv::Mat();
    }

    // Set up a filtering stream to unzip the file
    boost::iostreams::filtering_istream bunzip2Filter;
    bunzip2Filter.push(boost::iostreams::bzip2_decompressor());
    bunzip2Filter.push(bz2Input);

    // Convert the stream to an OpenCV Mat
    std::vector<char> tmp;
    boost::iostreams::copy(bunzip2Filter, boost::iostreams::back_inserter(tmp));
    return cv::imdecode(tmp, cv::IMREAD_COLOR);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: ./feret_demo PATH" << endl;
        cerr << "PATH: Path to color FERET directory, with final \"/\"." << endl;
        return EXIT_FAILURE;
    }

    string imgFilePath = argv[1];

    try {
        for (int i = 1; i <= 1208; i++) {
            int dvdNum = (i < 740) ? 1 : 2;

            std::ostringstream subjectStream;
            subjectStream << imgFilePath << "dvd" << dvdNum << "/data/images/"
                          << std::setw(5) << std::setfill('0') << i;
            boost::filesystem::path subjectDir(subjectStream.str());

            if (!boost::filesystem::exists(subjectDir)) {
                cout << "Could not find subject " << i << " directory " << subjectDir << endl;
                continue;
            }

            if (boost::filesystem::is_regular_file(subjectDir)) {
                cout << "Ignoring file (expected directory) " << subjectDir << endl;
                continue;
            } else if (boost::filesystem::is_directory(subjectDir)) {
                for (boost::filesystem::directory_entry entry :
                        boost::make_iterator_range(boost::filesystem::directory_iterator(subjectDir))) {

                    if (boost::filesystem::extension(entry.path()) == ".bz2") {
                        cv::Mat image = imreadBz2(entry.path().string());
                        if (!image.empty()) {
                            //cout << "Read in " << entry.path() << endl;
                            cv::imshow("Extracted Image", image);
                            cv::waitKey(300);
                        }
                    }
                }
            } else {
                cout << subjectDir << " exists, but is neither a regular file nor a directory" << endl;
            }
        }
    } catch (const boost::filesystem::filesystem_error& ex) {
        cout << ex.what() << '\n';
    }

    cv::destroyWindow("Extracted Image");

    return EXIT_SUCCESS;
}

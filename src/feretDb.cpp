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

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: ./feret_demo PATH" << endl;
        cerr << "PATH: Path to a bz2-compressed image file." << endl;
        //cerr << "PATH: Path to the COLORFERET directory (with ending /)" << endl;
        return EXIT_FAILURE;
    }

    string imgFilePath = argv[1];
    boost::filesystem::path imgPath(imgFilePath);

    if (boost::filesystem::exists(imgPath) && boost::filesystem::extension(imgPath) == ".bz2") {
        boost::iostreams::file_source bz2Input(imgFilePath, std::ios_base::in | std::ios_base::binary);

        // Set up a filtering stream to unzip the file
        boost::iostreams::filtering_istream bunzip2Filter;
        bunzip2Filter.push(boost::iostreams::bzip2_decompressor());
        bunzip2Filter.push(bz2Input);

        // Convert the stream to an OpenCV Mat
        std::vector<char> tmp;
        boost::iostreams::copy(bunzip2Filter, boost::iostreams::back_inserter(tmp));
        cv::Mat image = cv::imdecode(tmp, cv::IMREAD_GRAYSCALE);

        // Display image for three seconds
        cout << "Matrix size: " << image.size() << endl;
        cv::imshow("Extracted Image", image);
        cv::waitKey(3000);
        cv::destroyWindow("Extracted Image");
    } else {
        cout << "Invalid input: " << imgFilePath << endl;
    }

    return EXIT_SUCCESS;
}

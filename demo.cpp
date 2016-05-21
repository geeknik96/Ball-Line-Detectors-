#include <Vision.h>
#include <boost/exception/all.hpp>

int main(int argc, char **argv) {
    const std::string config_path = argv[1];
    const std::string save_dir = argv[2];
    int i = 0;
    while (true)
    {
        try {
            boost::property_tree::ptree ptree;
            boost::property_tree::json_parser::read_json(config_path, ptree);

            rd::Vision vision(ptree);

            std::string path = save_dir + "/" + std::to_string(i) + ".png";
            cv::Mat image = cv::imread(path), orginal_image;

            cv::Mat yuvImage;
            cv::cvtColor(image, yuvImage, CV_BGR2YUV);
            vision.setFrame(yuvImage);

            auto rect = vision.ballDetect();
            auto lines = vision.lineDetect();

            std::cout << i << ' ' << rect << std::endl;
            orginal_image = image.clone();
            cv::rectangle(image, rect, cv::Scalar(255, 0, 255));
            for (auto &line : lines) {
                const cv::Point p1(line(0), line(1));
                const cv::Point p2(line(2), line(3));
                cv::line(image, p1, p2, cv::Scalar(0, 0, 255));
            }

            cv::imshow("result", image);

            int key = cv::waitKey();

            if (key == 27) break;
            else if (key == '1') i--;
            else if (key == '2') i++;
        }
        catch (const boost::exception & exception) {
            std::cerr << boost::diagnostic_information(exception);
            return EXIT_FAILURE;
        }
        catch (...) {
            std::cerr << "Some error" << std::endl;
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
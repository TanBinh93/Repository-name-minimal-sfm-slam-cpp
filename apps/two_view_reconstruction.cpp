#include "pointcloud/ply_io.hpp"
#include "sfm/two_view_sfm.hpp"

#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <filesystem>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <image1.jpg> <image2.jpg> <intrinsics.yaml>\n";
        return 1;
    }

    const std::string image1_path = argv[1];
    const std::string image2_path = argv[2];
    const std::string intrinsics_path = argv[3];

    cv::Mat image1 = cv::imread(image1_path, cv::IMREAD_COLOR);
    cv::Mat image2 = cv::imread(image2_path, cv::IMREAD_COLOR);
    if (image1.empty() || image2.empty()) {
        std::cerr << "Failed to load input images." << std::endl;
        return 1;
    }

    cv::FileStorage fs(intrinsics_path, cv::FileStorage::READ);
    if (!fs.isOpened()) {
        std::cerr << "Failed to open intrinsics file: " << intrinsics_path << std::endl;
        return 1;
    }

    cv::Mat K;
    fs["camera_matrix"] >> K;
    if (K.empty()) {
        fs["K"] >> K;
    }
    if (K.empty()) {
        std::cerr << "Camera intrinsics not found in YAML." << std::endl;
        return 1;
    }

    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    std::vector<cv::DMatch> matches;
    minimal_sfm::ReconstructionResult reconstruction = minimal_sfm::runTwoViewSfm(
        image1, image2, K, keypoints1, keypoints2, matches);

    std::cout << "Detected keypoints image 1: " << keypoints1.size() << "\n";
    std::cout << "Detected keypoints image 2: " << keypoints2.size() << "\n";
    std::cout << "Filtered matches: " << matches.size() << "\n";
    std::cout << "Triangulated points: " << reconstruction.points.size() << "\n";

    if (reconstruction.points.empty()) {
        std::cerr << "Reconstruction failed or produced no 3D points." << std::endl;
        return 1;
    }

    const std::string ply_path = "output/sparse_cloud.ply";
    std::error_code ec;
    std::filesystem::create_directories("output", ec);
    if (!minimal_sfm::writePLY(ply_path, reconstruction.points)) {
        std::cerr << "Failed to write PLY file." << std::endl;
        return 1;
    }

    cv::Mat match_img;
    cv::drawMatches(image1, keypoints1, image2, keypoints2, reconstruction.inlier_matches, match_img);
    cv::imwrite("output/matches.png", match_img);

    std::cout << "Saved sparse cloud: " << ply_path << std::endl;
    std::cout << "Saved matches visualization: output/matches.png" << std::endl;

    return 0;
}

#pragma once

#include <array>
#include <opencv2/core.hpp>
#include <vector>

namespace minimal_sfm {

struct ReconstructionResult {
    std::vector<std::array<double, 3>> points;
    cv::Mat rotation;
    cv::Mat translation;
    std::vector<cv::DMatch> inlier_matches;
};

ReconstructionResult runTwoViewSfm(
    const cv::Mat& image1,
    const cv::Mat& image2,
    const cv::Mat& intrinsics,
    std::vector<cv::KeyPoint>& keypoints1,
    std::vector<cv::KeyPoint>& keypoints2,
    std::vector<cv::DMatch>& out_matches);

} // namespace minimal_sfm

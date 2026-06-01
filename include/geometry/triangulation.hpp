#pragma once

#include <array>
#include <opencv2/core.hpp>
#include <vector>

namespace minimal_sfm {

std::vector<std::array<double, 3>> triangulatePoints(
    const cv::Mat& P1,
    const cv::Mat& P2,
    const std::vector<cv::Point2f>& points1,
    const std::vector<cv::Point2f>& points2);

} // namespace minimal_sfm

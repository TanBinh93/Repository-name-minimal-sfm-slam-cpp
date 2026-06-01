#pragma once

#include <opencv2/core.hpp>
#include <vector>

namespace minimal_sfm {

struct MatchResult {
    std::vector<cv::DMatch> matches;
};

MatchResult matchFeatures(const cv::Mat& descriptors1, const cv::Mat& descriptors2);

} // namespace minimal_sfm

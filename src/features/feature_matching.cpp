#include "features/feature_matching.hpp"

#include <opencv2/features2d.hpp>

namespace minimal_sfm {

MatchResult matchFeatures(const cv::Mat& descriptors1, const cv::Mat& descriptors2) {
    MatchResult result;
    if (descriptors1.empty() || descriptors2.empty()) {
        return result;
    }

    int norm_type = descriptors1.type() == CV_8U ? cv::NORM_HAMMING : cv::NORM_L2;
    cv::BFMatcher matcher(norm_type);
    std::vector<std::vector<cv::DMatch>> knn_matches;
    matcher.knnMatch(descriptors1, descriptors2, knn_matches, 2);

    const float ratio_thresh = 0.75f;
    for (const auto& knn : knn_matches) {
        if (knn.size() < 2) {
            continue;
        }
        if (knn[0].distance < ratio_thresh * knn[1].distance) {
            result.matches.push_back(knn[0]);
        }
    }
    return result;
}

} // namespace minimal_sfm

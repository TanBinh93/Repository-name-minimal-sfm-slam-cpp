#include "geometry/triangulation.hpp"
#include <opencv2/calib3d.hpp>

namespace minimal_sfm {

std::vector<std::array<double, 3>> triangulatePoints(
    const cv::Mat& P1,
    const cv::Mat& P2,
    const std::vector<cv::Point2f>& points1,
    const std::vector<cv::Point2f>& points2) {
    cv::Mat homogeneous;
    cv::triangulatePoints(P1, P2, points1, points2, homogeneous);

    std::vector<std::array<double, 3>> output;
    output.reserve(homogeneous.cols);
    for (int i = 0; i < homogeneous.cols; ++i) {
        cv::Mat col = homogeneous.col(i);
        double w = col.at<double>(3, 0);
        if (std::abs(w) < 1e-8) {
            continue;
        }
        output.push_back({
            col.at<double>(0, 0) / w,
            col.at<double>(1, 0) / w,
            col.at<double>(2, 0) / w,
        });
    }
    return output;
}

} // namespace minimal_sfm

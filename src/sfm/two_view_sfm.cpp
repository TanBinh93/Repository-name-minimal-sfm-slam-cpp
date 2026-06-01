#include "features/feature_matching.hpp"
#include "geometry/triangulation.hpp"
#include "sfm/two_view_sfm.hpp"

#include <opencv2/calib3d.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>

namespace minimal_sfm {

static std::vector<cv::DMatch> filterMatches(const std::vector<std::vector<cv::DMatch>>& knn_matches) {
    std::vector<cv::DMatch> filtered;
    const float ratio_thresh = 0.75f;
    for (const auto& knn : knn_matches) {
        if (knn.size() < 2) {
            continue;
        }
        const cv::DMatch& m1 = knn[0];
        const cv::DMatch& m2 = knn[1];
        if (m1.distance < ratio_thresh * m2.distance) {
            filtered.push_back(m1);
        }
    }
    return filtered;
}

ReconstructionResult runTwoViewSfm(
    const cv::Mat& image1,
    const cv::Mat& image2,
    const cv::Mat& intrinsics,
    std::vector<cv::KeyPoint>& keypoints1,
    std::vector<cv::KeyPoint>& keypoints2,
    std::vector<cv::DMatch>& out_matches) {
    ReconstructionResult result;

    cv::Ptr<cv::ORB> orb = cv::ORB::create(2000);
    cv::Mat descriptors1, descriptors2;
    orb->detectAndCompute(image1, {}, keypoints1, descriptors1);
    orb->detectAndCompute(image2, {}, keypoints2, descriptors2);

    if (descriptors1.empty() || descriptors2.empty()) {
        return result;
    }

    minimal_sfm::MatchResult match_result = minimal_sfm::matchFeatures(descriptors1, descriptors2);
    out_matches = std::move(match_result.matches);

    if (out_matches.size() < 8) {
        return result;
    }

    std::vector<cv::Point2f> points1;
    std::vector<cv::Point2f> points2;
    points1.reserve(out_matches.size());
    points2.reserve(out_matches.size());
    for (const auto& match : out_matches) {
        points1.push_back(keypoints1[match.queryIdx].pt);
        points2.push_back(keypoints2[match.trainIdx].pt);
    }

    cv::Mat essential;
    std::vector<unsigned char> inliers_mask;
    essential = cv::findEssentialMat(points1, points2, intrinsics, cv::RANSAC, 0.999, 1.0, inliers_mask);
    if (essential.empty()) {
        return result;
    }

    cv::Mat R, t;
    int inliers = cv::recoverPose(essential, points1, points2, intrinsics, R, t, inliers_mask);
    if (inliers < 8) {
        return result;
    }

    std::vector<cv::Point2f> inlier_points1;
    std::vector<cv::Point2f> inlier_points2;
    std::vector<cv::DMatch> inlier_matches;
    inlier_points1.reserve(inliers);
    inlier_points2.reserve(inliers);
    inlier_matches.reserve(inliers);

    for (size_t i = 0; i < out_matches.size(); ++i) {
        if (i < inliers_mask.size() && inliers_mask[i]) {
            inlier_points1.push_back(points1[i]);
            inlier_points2.push_back(points2[i]);
            inlier_matches.push_back(out_matches[i]);
        }
    }

    cv::Mat P1 = intrinsics * cv::Mat::eye(3, 4, CV_64F);
    cv::Mat P2 = cv::Mat::zeros(3, 4, CV_64F);
    cv::Mat R64, t64;
    R.convertTo(R64, CV_64F);
    t.convertTo(t64, CV_64F);
    R64.copyTo(P2(cv::Rect(0, 0, 3, 3)));
    t64.copyTo(P2(cv::Rect(3, 0, 1, 3)));
    P2 = intrinsics * P2;

    result.points = triangulatePoints(P1, P2, inlier_points1, inlier_points2);
    result.rotation = R.clone();
    result.translation = t.clone();
    result.inlier_matches = std::move(inlier_matches);
    return result;
}

} // namespace minimal_sfm

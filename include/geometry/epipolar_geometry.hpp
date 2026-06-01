#pragma once

#include <opencv2/core.hpp>

namespace minimal_sfm {

struct EpipolarGeometry {
    cv::Mat essential;
    cv::Mat fundamental;
};

} // namespace minimal_sfm

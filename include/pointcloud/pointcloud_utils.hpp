#pragma once

#include <array>
#include <vector>

namespace minimal_sfm {

std::vector<std::array<double, 3>> downsamplePointCloud(const std::vector<std::array<double, 3>>& points);

} // namespace minimal_sfm

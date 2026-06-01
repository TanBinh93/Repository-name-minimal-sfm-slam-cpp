#pragma once

#include <array>

namespace minimal_sfm {

std::array<double, 16> poseFromRt(const std::array<double, 9>& R, const std::array<double, 3>& t);

} // namespace minimal_sfm

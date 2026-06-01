#pragma once

#include <array>
#include <vector>

namespace minimal_sfm {

struct TrajectoryPoint {
    double timestamp = 0.0;
    std::array<double, 3> position = {0.0, 0.0, 0.0};
};

std::vector<TrajectoryPoint> runMonocularVo();

} // namespace minimal_sfm

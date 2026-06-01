#pragma once

#include <array>

namespace minimal_sfm {

struct PinholeCamera {
    std::array<double, 9> intrinsics = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    std::array<double, 16> pose = {1.0, 0.0, 0.0, 0.0,
                                  0.0, 1.0, 0.0, 0.0,
                                  0.0, 0.0, 1.0, 0.0,
                                  0.0, 0.0, 0.0, 1.0};
};

} // namespace minimal_sfm

#pragma once

#include <vector>

namespace minimal_sfm {

struct BundleAdjustmentResult {
    double initial_error = 0.0;
    double final_error = 0.0;
};

BundleAdjustmentResult optimizeBundleAdjustment();

} // namespace minimal_sfm

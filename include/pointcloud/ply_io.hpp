#pragma once

#include <array>
#include <string>
#include <vector>

namespace minimal_sfm {

bool writePLY(const std::string& filename, const std::vector<std::array<double, 3>>& points);
bool readPLY(const std::string& filename, std::vector<std::array<double, 3>>& points);

} // namespace minimal_sfm

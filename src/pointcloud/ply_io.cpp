#include "pointcloud/ply_io.hpp"

#include <fstream>

namespace minimal_sfm {

bool writePLY(const std::string& filename, const std::vector<std::array<double, 3>>& points) {
    std::ofstream out(filename, std::ios::out);
    if (!out.is_open()) {
        return false;
    }

    out << "ply\n";
    out << "format ascii 1.0\n";
    out << "element vertex " << points.size() << "\n";
    out << "property float x\n";
    out << "property float y\n";
    out << "property float z\n";
    out << "end_header\n";
    for (const auto& point : points) {
        out << point[0] << " " << point[1] << " " << point[2] << "\n";
    }
    return true;
}

bool readPLY(const std::string& filename, std::vector<std::array<double, 3>>& points) {
    std::ifstream in(filename, std::ios::in);
    if (!in.is_open()) {
        return false;
    }

    std::string line;
    size_t vertex_count = 0;
    while (std::getline(in, line)) {
        if (line.rfind("element vertex", 0) == 0) {
            std::sscanf(line.c_str(), "element vertex %zu", &vertex_count);
        }
        if (line == "end_header") {
            break;
        }
    }

    points.clear();
    points.reserve(vertex_count);
    for (size_t i = 0; i < vertex_count; ++i) {
        double x, y, z;
        if (!(in >> x >> y >> z)) {
            break;
        }
        points.push_back({x, y, z});
    }
    return !points.empty();
}

} // namespace minimal_sfm

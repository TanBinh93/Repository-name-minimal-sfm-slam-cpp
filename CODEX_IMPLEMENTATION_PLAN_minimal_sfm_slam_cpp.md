# CODEX IMPLEMENTATION PLAN — Minimal SfM / SLAM / 3D Reconstruction C++ Project

## 0. Project Goal

Build a clean, modern C++17 GitHub repository demonstrating core skills for a SLAM / XR / 3D Computer Vision Engineer role.

The repository should show:

- Structure from Motion
- Two-view geometry
- Feature matching
- Camera pose recovery
- Triangulation
- Bundle Adjustment
- Point cloud export and processing
- Basic visual odometry structure
- Clean C++ design
- CMake build system
- Reproducible demos
- Clear documentation

This project is not intended to replace COLMAP, ORB-SLAM, OpenMVG, or Open3D.  
It is intended to demonstrate that the author understands the mathematical and software foundations of SLAM and 3D reconstruction.

Target role: SLAM Engineer / XR Perception / Computer Vision Engineer.

---

## 1. Repository Name Suggestion

Use one of these names:

```text
minimal-sfm-slam-cpp
geometry-vision-lab
cpp-sfm-vo-lab
```

Recommended:

```text
minimal-sfm-slam-cpp
```

---

## 2. Technology Stack

Use:

```text
C++17
CMake
OpenCV
Eigen
Ceres Solver
Optional: PCL or Open3D later
Optional: GoogleTest later
```

Do not over-engineer the first version.

Priority:

1. The project must compile.
2. The demos must run.
3. The README must show visual results.
4. The code must be readable.

---

## 3. Target Repository Structure

Create this structure:

```text
minimal-sfm-slam-cpp/
├── README.md
├── CMakeLists.txt
├── .gitignore
├── data/
│   ├── README.md
│   └── sample/
├── include/
│   ├── camera/
│   │   └── pinhole_camera.hpp
│   ├── geometry/
│   │   ├── epipolar_geometry.hpp
│   │   ├── triangulation.hpp
│   │   └── pose_utils.hpp
│   ├── features/
│   │   └── feature_matching.hpp
│   ├── sfm/
│   │   ├── two_view_sfm.hpp
│   │   └── bundle_adjustment.hpp
│   ├── pointcloud/
│   │   ├── ply_io.hpp
│   │   └── pointcloud_utils.hpp
│   └── vo/
│       └── monocular_vo.hpp
├── src/
│   ├── camera/
│   ├── geometry/
│   ├── features/
│   ├── sfm/
│   ├── pointcloud/
│   └── vo/
├── apps/
│   ├── two_view_reconstruction.cpp
│   ├── bundle_adjustment_demo.cpp
│   ├── pointcloud_demo.cpp
│   └── monocular_vo_demo.cpp
├── tests/
│   └── README.md
├── docs/
│   ├── math_notes.md
│   ├── results.md
│   └── roadmap.md
└── scripts/
    ├── visualize_pointcloud.py
    └── plot_trajectory.py
```

---

## 4. Phase 1 — Minimal Two-View Reconstruction

### Goal

Implement a complete two-view Structure from Motion demo.

Input:

```text
image_1.jpg
image_2.jpg
camera_intrinsics.yaml
```

Output:

```text
matches.png
epipolar_lines.png
sparse_cloud.ply
reconstruction_report.txt
```

### Required Pipeline

Implement the following steps:

1. Load two images.
2. Load camera intrinsics from YAML.
3. Detect keypoints using ORB by default.
4. Match descriptors using brute-force matching.
5. Filter matches using Lowe ratio test or cross-check.
6. Estimate Essential Matrix using RANSAC.
7. Recover relative camera pose `R, t`.
8. Triangulate 3D points.
9. Filter points:
   - positive depth in both cameras
   - reasonable reprojection error
10. Export sparse 3D points to `.ply`.
11. Save visualizations:
   - feature matches
   - epipolar lines
12. Print reconstruction statistics.

### Expected Console Output

Example:

```text
Loaded image 1: data/sample/img_001.jpg
Loaded image 2: data/sample/img_002.jpg
Detected keypoints image 1: 2534
Detected keypoints image 2: 2412
Raw matches: 1200
Filtered matches: 431
Essential matrix inliers: 287
Triangulated points: 287
Valid 3D points: 198
Mean reprojection error before BA: 2.73 px
Saved sparse cloud: output/sparse_cloud.ply
```

---

## 5. Phase 2 — Bundle Adjustment

### Goal

Implement a small Bundle Adjustment module using Ceres Solver.

### Required Features

Implement:

- Reprojection residual
- Camera pose parameterization
- 3D point optimization
- Intrinsics fixed in the first version
- Report reprojection error before and after optimization

### File Targets

Create:

```text
include/sfm/bundle_adjustment.hpp
src/sfm/bundle_adjustment.cpp
apps/bundle_adjustment_demo.cpp
```

### Mathematical Model

For each observation:

```text
X_world -> camera pose -> normalized camera coordinates -> pixel coordinates
```

Minimize reprojection error:

```text
sum || observed_pixel - projected_pixel ||^2
```

### Expected Output

```text
Bundle Adjustment
Number of cameras: 2
Number of 3D points: 198
Number of observations: 396
Initial mean reprojection error: 2.73 px
Final mean reprojection error: 0.84 px
```

---

## 6. Phase 3 — Point Cloud Processing

### Goal

Add basic point cloud utilities relevant to 3D reconstruction and SLAM.

### Required Features

Implement:

- PLY writer
- PLY reader
- Voxel downsampling
- Statistical outlier removal
- RANSAC plane fitting
- Simple ICP registration if possible

### File Targets

```text
include/pointcloud/ply_io.hpp
include/pointcloud/pointcloud_utils.hpp
src/pointcloud/ply_io.cpp
src/pointcloud/pointcloud_utils.cpp
apps/pointcloud_demo.cpp
```

### Expected Demo

Input:

```text
output/sparse_cloud.ply
```

Output:

```text
output/sparse_cloud_downsampled.ply
output/sparse_cloud_cleaned.ply
```

---

## 7. Phase 4 — Monocular Visual Odometry Skeleton

### Goal

Create a simple monocular visual odometry pipeline.

This does not need to be a full SLAM system.  
The goal is to show architecture and understanding.

### Required Features

Implement:

- Sequential frame loading
- Feature tracking between consecutive frames
- Essential matrix estimation
- Relative pose recovery
- Pose accumulation
- Trajectory export in TUM-like format

### File Targets

```text
include/vo/monocular_vo.hpp
src/vo/monocular_vo.cpp
apps/monocular_vo_demo.cpp
```

### Expected Output

```text
output/trajectory.txt
output/trajectory.png
```

Trajectory format:

```text
timestamp tx ty tz qx qy qz qw
```

---

## 8. C++ Style Requirements

Use:

```text
C++17
RAII
const correctness
std::vector
std::array
std::optional where useful
smart pointers only when ownership is needed
Eigen::Matrix types for math
OpenCV only for image processing and feature extraction
```

Avoid:

```text
global variables
large monolithic functions
hard-coded absolute paths
unexplained magic numbers
unnecessary templates
```

Each module should have clear responsibility.

---

## 9. CMake Requirements

Create a clean `CMakeLists.txt`.

Minimum requirements:

```cmake
cmake_minimum_required(VERSION 3.16)
project(minimal_sfm_slam_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(Ceres REQUIRED)
```

Build apps:

```text
two_view_reconstruction
bundle_adjustment_demo
pointcloud_demo
monocular_vo_demo
```

---

## 10. README Requirements

The README is extremely important.

Create a professional README with:

1. Project title
2. Short description
3. Why this project exists
4. Implemented features
5. Build instructions
6. How to run demos
7. Example results
8. Roadmap
9. Relation to SLAM / XR perception
10. References

### README Opening Example

Use this style:

```markdown
# Minimal SfM / SLAM C++ Lab

This repository implements core building blocks of 3D computer vision and SLAM in modern C++17: two-view geometry, camera pose recovery, triangulation, bundle adjustment, point cloud export, and a minimal monocular visual odometry pipeline.

The goal is educational and demonstrative: to provide clean, readable implementations of the mathematical components behind Structure from Motion and visual SLAM systems.
```

---

## 11. Documentation Requirements

Create:

```text
docs/math_notes.md
```

Include explanations of:

- Pinhole camera model
- Essential matrix
- Epipolar constraint
- Triangulation
- Reprojection error
- Bundle Adjustment
- Visual odometry

Create:

```text
docs/results.md
```

Include:

- screenshots
- output point cloud examples
- reprojection error before and after BA
- limitations

Create:

```text
docs/roadmap.md
```

Include future improvements:

- loop closure
- local mapping
- IMU fusion
- visual-inertial odometry
- real-time optimization
- mobile/XR deployment

---

## 12. Dataset Policy

Do not commit large datasets.

In `data/README.md`, explain how to use:

- personal images
- small image pairs
- optional public datasets

Suggested optional datasets:

```text
KITTI Odometry
EuRoC MAV
TUM RGB-D
ETH3D
```

But the first version should work with two user-provided images.

---

## 13. Output Folder

Create an `output/` folder automatically if it does not exist.

Do not commit generated output files unless they are small and useful for README illustration.

Add to `.gitignore`:

```text
/build/
/output/
*.ply
*.log
.DS_Store
.vscode/
```

---

## 14. First CODEX Task

Start with this task:

```text
Create the initial repository structure, CMakeLists.txt, README.md, data/README.md, docs/math_notes.md, docs/roadmap.md, and empty header/source files for the modules listed above.
Do not implement the full algorithms yet.
Make sure the project configures with CMake.
```

---

## 15. Second CODEX Task

After the structure is created, implement:

```text
apps/two_view_reconstruction.cpp
include/features/feature_matching.hpp
src/features/feature_matching.cpp
include/geometry/triangulation.hpp
src/geometry/triangulation.cpp
include/sfm/two_view_sfm.hpp
src/sfm/two_view_sfm.cpp
```

The app should:

```text
./two_view_reconstruction image1.jpg image2.jpg intrinsics.yaml
```

---

## 16. Third CODEX Task

Implement Bundle Adjustment using Ceres.

Keep intrinsics fixed.

Optimize:

```text
camera pose
3D points
```

Report:

```text
mean reprojection error before optimization
mean reprojection error after optimization
```

---

## 17. Fourth CODEX Task

Add point cloud utilities:

```text
PLY export
PLY import
voxel downsampling
outlier removal
```

---

## 18. Fifth CODEX Task

Add a simple monocular VO demo:

```text
./monocular_vo_demo path_to_image_sequence intrinsics.yaml
```

Export:

```text
output/trajectory.txt
```

---

## 19. Important Instruction for CODEX

Do not generate a huge amount of code at once.

Work incrementally:

1. Create structure.
2. Make CMake compile.
3. Add one module.
4. Compile again.
5. Add one demo.
6. Compile again.
7. Update README after every working feature.

Every commit should compile.

---

## 20. Git Commit Plan

Suggested commits:

```text
git commit -m "Initialize C++ SfM SLAM project structure"
git commit -m "Add two-view feature matching demo"
git commit -m "Add essential matrix pose recovery and triangulation"
git commit -m "Add PLY export for sparse reconstruction"
git commit -m "Add bundle adjustment with Ceres"
git commit -m "Add point cloud processing utilities"
git commit -m "Add monocular visual odometry skeleton"
git commit -m "Improve documentation and example results"
```

---

## 21. What This Project Should Demonstrate to Recruiters

This project should make clear that the author can work on:

- SLAM
- 3D computer vision
- XR perception
- geometric vision
- optimization
- C++ software engineering
- point cloud processing
- reconstruction pipelines

This is directly relevant to roles in:

- AR/VR/XR perception
- Visual SLAM
- Robotics perception
- 3D mapping
- Computer vision R&D

---

## 22. Definition of Done for Version 1

Version 1 is done when:

- The project builds with CMake.
- `two_view_reconstruction` runs on two images.
- The app exports a sparse `.ply` point cloud.
- The app reports reconstruction statistics.
- The README contains screenshots and command examples.
- The code is clean enough to show on GitHub.

Do not wait until the project is perfect before publishing.

A small working project is better than a large unfinished one.

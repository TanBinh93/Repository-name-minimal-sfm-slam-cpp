# Minimal Structure-from-Motion and Geometric Vision in Modern C++

> Modern C++17 implementation of two-view Structure-from-Motion components for SLAM, XR perception, and geometric vision.

## Project overview

This repository demonstrates a clean C++ architecture for core 3D vision building blocks:

- ORB feature extraction
- Descriptor matching and inlier filtering
- Essential matrix estimation and relative pose recovery
- Sparse triangulation
- Point cloud export to PLY

The code is intended for recruiters and hiring managers who want to see working geometry and SLAM fundamentals in C++.

## Implemented features

- ORB keypoint detection and descriptor extraction
- Brute-force descriptor matching with Lowe ratio test
- Essential matrix estimation using RANSAC
- Relative pose recovery (`R`, `t`) from two views
- Triangulation of sparse 3D points
- Export of sparse point cloud to `output/sparse_cloud.ply`
- Match visualization saved to `output/matches.png`

## In progress

- Bundle adjustment using Ceres Solver
- Point cloud processing utilities
- Monocular visual odometry skeleton

## Build instructions

### Prerequisites

- CMake 3.16+
- C++17 compiler (MSVC, GCC, Clang)
- OpenCV
- Optional: Ceres Solver for future bundle adjustment work

### Build

```bash
mkdir build
cd build
cmake -S .. -B .
cmake --build . --config Release
```

### Run the two-view demo

```bash
./build/Release/two_view_reconstruction image1.jpg image2.jpg intrinsics.yaml
```

## Example outputs

- `output/sparse_cloud.ply` — sparse 3D reconstruction
- `output/matches.png` — matched feature pairs between the two images

> Add your own sample image pair and camera intrinsics file to exercise the demo.

## Architecture

```text
apps/two_view_reconstruction.cpp
  └── uses minimal_sfm_lib
        ├── features/feature_matching.cpp
        ├── sfm/two_view_sfm.cpp
        │     ├── epipolar geometry
        │     ├── pose recovery
        │     └── triangulation
        └── pointcloud/ply_io.cpp
```

## Project structure

- `CMakeLists.txt` — build system
- `include/` — public interfaces
- `src/` — module implementations
- `apps/` — runnable demos
- `docs/` — project notes and roadmap
- `data/` — data usage instructions
- `scripts/` — helper scripts

## Project Goals

This project is a clean, early-stage implementation of two-view SfM and geometric vision fundamentals in C++. It is designed to show:

- practical C++ software architecture
- OpenCV-based computer vision pipelines
- mathematical understanding of epipolar geometry and triangulation
- a roadmap toward SLAM / visual odometry features

## Roadmap

- Complete Ceres-based bundle adjustment
- Add PLY reading and point cloud processing
- Implement monocular visual odometry trajectory output
- Add unit tests and dataset examples
- Improve results reporting and visualization

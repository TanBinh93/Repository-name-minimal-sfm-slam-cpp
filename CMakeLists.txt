cmake_minimum_required(VERSION 3.16)
project(minimal_sfm_slam_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(OpenCV REQUIRED)
find_package(Ceres QUIET)

if (NOT CERES_FOUND)
  message(WARNING "Ceres not found. Bundle adjustment demo will be built without Ceres.")
endif()

add_library(minimal_sfm_lib
  src/features/feature_matching.cpp
  src/geometry/triangulation.cpp
  src/sfm/two_view_sfm.cpp
  src/sfm/bundle_adjustment.cpp
  src/pointcloud/ply_io.cpp
  src/pointcloud/pointcloud_utils.cpp
  src/vo/monocular_vo.cpp
)

target_include_directories(minimal_sfm_lib PUBLIC include ${OpenCV_INCLUDE_DIRS})
target_link_libraries(minimal_sfm_lib PUBLIC ${OpenCV_LIBS})

if (CERES_FOUND)
  target_link_libraries(minimal_sfm_lib PUBLIC ${CERES_LIBRARIES})
endif()

add_executable(two_view_reconstruction apps/two_view_reconstruction.cpp)
target_link_libraries(two_view_reconstruction PRIVATE minimal_sfm_lib)

add_executable(bundle_adjustment_demo apps/bundle_adjustment_demo.cpp)
target_link_libraries(bundle_adjustment_demo PRIVATE minimal_sfm_lib)

add_executable(pointcloud_demo apps/pointcloud_demo.cpp)
target_link_libraries(pointcloud_demo PRIVATE minimal_sfm_lib)

add_executable(monocular_vo_demo apps/monocular_vo_demo.cpp)
target_link_libraries(monocular_vo_demo PRIVATE minimal_sfm_lib)


cmake_minimum_required(VERSION 2.8)
 
PROJECT(PointToSphere)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(PointToSphere MACOSX_BUNDLE PointToSphere.cxx )
 
target_link_libraries(PointToSphere ${VTK_LIBRARIES})

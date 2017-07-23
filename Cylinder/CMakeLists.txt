
cmake_minimum_required(VERSION 2.8)
 
PROJECT(Cylinder)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(Cylinder MACOSX_BUNDLE Cylinder.cxx )
 
target_link_libraries(Cylinder ${VTK_LIBRARIES})

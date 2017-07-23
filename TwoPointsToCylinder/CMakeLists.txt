
cmake_minimum_required(VERSION 2.8)
 
PROJECT(OrientedCylinder)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(OrientedCylinder MACOSX_BUNDLE OrientedCylinder.cxx )
 
target_link_libraries(OrientedCylinder ${VTK_LIBRARIES})


cmake_minimum_required(VERSION 2.8)
 
PROJECT(GraphProcessing)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(GraphProcessing MACOSX_BUNDLE GraphProcessing.cxx )
 
target_link_libraries(GraphProcessing ${VTK_LIBRARIES})

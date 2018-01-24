
cmake_minimum_required(VERSION 2.8)
 
PROJECT(GraphToVTP)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(GraphToVTP MACOSX_BUNDLE GraphToVTP.cxx )
 
target_link_libraries(GraphToVTP ${VTK_LIBRARIES})

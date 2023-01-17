
cmake_minimum_required(VERSION 2.8)
 
PROJECT(CollisionDetection)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )

add_executable(CollisionDetection MACOSX_BUNDLE CollisionDetection.cxx )
 
target_link_libraries(CollisionDetection ${VTK_LIBRARIES})

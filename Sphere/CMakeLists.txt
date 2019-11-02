
cmake_minimum_required(VERSION 2.8)

PROJECT(Sphere)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )

add_executable(Sphere MACOSX_BUNDLE Sphere.cxx )

target_link_libraries(Sphere ${VTK_LIBRARIES})

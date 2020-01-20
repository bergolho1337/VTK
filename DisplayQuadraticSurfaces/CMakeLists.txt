
cmake_minimum_required(VERSION 2.8)

PROJECT(Paraboloid)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )

add_executable(Paraboloid MACOSX_BUNDLE Paraboloid.cxx )

target_link_libraries(Paraboloid ${VTK_LIBRARIES})

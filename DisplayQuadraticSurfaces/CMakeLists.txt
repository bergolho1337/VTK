
cmake_minimum_required(VERSION 2.8)

PROJECT(Quadratic)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )

add_executable(Quadratic MACOSX_BUNDLE Quadratic.cxx )

target_link_libraries(Quadratic ${VTK_LIBRARIES})

cmake_minimum_required(VERSION 2.8)

PROJECT(VTU_Utils)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )

add_executable(VTU_Utils MACOSX_BUNDLE main.cpp )

target_link_libraries(VTU_Utils ${VTK_LIBRARIES})

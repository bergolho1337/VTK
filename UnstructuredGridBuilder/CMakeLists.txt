cmake_minimum_required(VERSION 2.8)
 
PROJECT(UnstructuredGridBuilder)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )

ADD_SUBDIRECTORY(src/utils)

ADD_EXECUTABLE(UnstructuredGridBuilder src/main.cpp )

ADD_DEPENDENCIES(UnstructuredGridBuilder utils)
 
TARGET_LINK_LIBRARIES(UnstructuredGridBuilder ${VTK_LIBRARIES} utils)

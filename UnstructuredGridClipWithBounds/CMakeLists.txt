cmake_minimum_required(VERSION 2.8)
 
PROJECT(UnstructuredGridClipWithBounds)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )

ADD_SUBDIRECTORY(src/utils)
ADD_SUBDIRECTORY(src/tissue)

ADD_EXECUTABLE(UnstructuredGridClipWithBounds src/main.cpp )

ADD_DEPENDENCIES(UnstructuredGridClipWithBounds utils tissue)
 
TARGET_LINK_LIBRARIES(UnstructuredGridClipWithBounds ${VTK_LIBRARIES} utils tissue)

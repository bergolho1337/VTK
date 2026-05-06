cmake_minimum_required(VERSION 3.12)
project(refine_tetra_vtu)

find_package(VTK REQUIRED)

add_executable(refine_tetra_vtu main.cpp)
target_link_libraries(refine_tetra_vtu PRIVATE ${VTK_LIBRARIES})
vtk_module_autoinit(
  TARGETS refine_tetra_vtu
  MODULES ${VTK_LIBRARIES}
)

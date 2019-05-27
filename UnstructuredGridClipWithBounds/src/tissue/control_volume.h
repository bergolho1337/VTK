#ifndef CONTROL_VOLUME_H
#define CONTROL_VOLUME_H

#include <iostream>
#include <string>
#include <map>

#include <cstdio>
#include <cstdlib>

#include <vtkXMLUnstructuredGridReader.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkUnstructuredGrid.h>
#include <vtkHexahedron.h>
#include <vtkSphereSource.h>
#include <vtkAppendPolyData.h>
#include <vtkFloatArray.h>
#include <vtkCellData.h>

#include "../utils/utils.h"

class Control_Volume
{
public:
    double center[3];
    
    double p1[3];
    double p2[3];
    double p3[3];
    double p4[3];
    double p5[3];
    double p6[3];
    double p7[3];
    double p8[3];
public:
    Control_Volume (const double center[]);
};

void read_data_inside_bounds_from_vtu(std::string input_file,\
                                const double min_x, const double max_x,\
                                const double min_y, const double max_y,\
                                const double min_z, const double max_z);

void calc_control_volume_center_position(vtkHexahedron *hexahedron, double center[]);
bool is_inside_bounds(const double center[],\
                    const double min_x, const double max_x,\
                    const double min_y, const double max_y,\
                    const double min_z, const double max_z);

#endif //MONOALG3D_UTILS_H_H
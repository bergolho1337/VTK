#ifndef CALC_PROPAGATION_VELOCITY_UTILS_H
#define CALC_PROPAGATION_VELOCITY_UTILS_H

#include <iostream>
#include <string>

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


#include "file_utils.h"

#define PRINT_LINE "====================================================================================================================="

#define DX 100
#define DY 100
#define DZ 100

void usage (const char pname[]);

class Point_3D
{
public:
    double x, y, z;
public:
    Point_3D (const double x, const double y, const double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    bool operator <(const Point_3D& pt) const
    {
        return (this->x < pt.x) || \
               ((!(pt.x < this->x)) && (this->y < pt.y)) || \
               ((!(pt.x < this->x)) && (!(pt.y < this->y)) && (this->z < pt.z));
    }
    friend std::ostream& operator<<(std::ostream& os, const Point_3D& obj);
};

void read_points_from_file (std::vector<Point_3D> &points, const std::string filename);

void write_vtu_from_points (std::vector<Point_3D> points);

#endif //MONOALG3D_UTILS_H_H
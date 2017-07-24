#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "vtkCellArray.h"
#include "vtkPoints.h"
#include "vtkLine.h"
#include "vtkPointData.h"
#include "vtkDoubleArray.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkPolyData.h"
#include <vtkVersion.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkPolyData.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkAppendPolyData.h>
#include <vtkFloatArray.h>
#include <vtkPolyDataMapper.h>
#include <vtkCylinderSource.h>
#include <vtkMath.h>
#include <vtkSphereSource.h>
#include <vtkProperty.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>


using namespace std;

struct Point;
struct Line;
void Usage (const char pName[]);
void readVTK (const char vtkName[], const char vtpName[]);
void writeVTP (const vector<Point> Points, const vector<Line> Lines, const char vtpName[]);

/* ---------------------------------------------------------------------- */
int main (int argc, char *argv[])
{
    if (argc-1 < 2)
    {
        Usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    else
    {
        readVTK(argv[1],argv[2]);
        return 0;
    }
}
/* ---------------------------------------------------------------------- */

struct Point
{
	double x,y,z;
    double scalar;
    Point () {}
	Point(const double xin, const double yin, const double zin, const double scalarin) : x(xin), y(yin), z(zin), scalar(scalarin) {}
};

struct Line
{
    int from;
    int to;
    Line () {}
    Line (const int fromin, const int toin) : from(fromin), to(toin) {}
};

/* ---------------------------------------------------------------------- */
void readVTK (const char vtkName[], const char vtpName[])
{
    int NumPoints, NumLines, trash;
    int line[2];
    double v[3];
    string str;
    vector<Point> Points;
    vector<Line> Lines;

    ifstream in(vtkName);
    while (in >> str && str != "POINTS");
    in >> NumPoints; in >> str;
    for (int i = 0; i < NumPoints; i++)
    {
        Point point;
        in >> point.x >> point.y >> point.z;
        Points.push_back(point);
    }
    while (in >> str && str != "LINES");
    in >> NumLines >> trash;
    for (int i = 0; i < NumLines; i++)
    {
        Line line;
        in >> trash >> line.from >> line.to;
        Lines.push_back(line);
    }
    while (in >> str && str != "default");
    for (int i = 0; i < NumPoints; i++)
        in >> Points[i].scalar;

    in.close();

    writeVTP(Points,Lines,vtpName);
}

void writeVTP (const vector<Point> Points, const vector<Line> Lines, const char vtpName[])
{
    // Create the polydata where we will store all the geometric data
    vtkSmartPointer<vtkPolyData> polydata =
        vtkSmartPointer<vtkPolyData>::New();

    // Create a vtkPoints container and store the points in it
    vtkSmartPointer<vtkPoints> pts =
        vtkSmartPointer<vtkPoints>::New();
    for (int i = 0; i < (int)Points.size(); i++)
    {
        double p[3];
        p[0] = Points[i].x; p[1] = Points[i].y; p[2] = Points[i].z;
        pts->InsertNextPoint(p);
    }
    // Add the points to the polydata container
    polydata->SetPoints(pts);

    // Create a vtkCellArray container and store the lines in it
    vtkSmartPointer<vtkCellArray> lines =
        vtkSmartPointer<vtkCellArray>::New();
    for (int i = 0; i < (int)Lines.size(); i++)
    {
        vtkSmartPointer<vtkLine> line =
            vtkSmartPointer<vtkLine>::New();
        line->GetPointIds()->SetId(0,Lines[i].from);
        line->GetPointIds()->SetId(1,Lines[i].to);
        lines->InsertNextCell(line);
    }
    
    // Add the lines to the polydata container
    polydata->SetLines(lines);

    // Set the scalars to the points
    vtkSmartPointer<vtkDoubleArray> scalars =
        vtkSmartPointer<vtkDoubleArray>::New();
    scalars->SetNumberOfValues((int)Points.size());
    for (int i = 0; i < (int)Points.size(); i++)
        scalars->SetValue(i,Points[i].scalar);
    polydata->GetPointData()->SetScalars(scalars);

    // Write the file
    vtkSmartPointer<vtkXMLPolyDataWriter> writer =  
        vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    writer->SetFileName(vtpName);
    #if VTK_MAJOR_VERSION <= 5
     writer->SetInput(polydata);
    #else
    writer->SetInputData(polydata);
    #endif
    //writer->SetInputConnection(polydata->GetOutputPort());
    writer->Write();
}

void Usage (const char pName[])
{
    cout << "=======================================================" << endl;
    cout << "Usage:> " << pName << " <in_vtk_file> <out_vtp_file>" << endl;
    cout << "=======================================================" << endl;
}
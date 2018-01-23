#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
 
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkPolyData.h"
#include <vtkVersion.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkSmartPointer.h>
#include <vtkDoubleArray.h>
#include <vtkAppendPolyData.h>
#include <vtkSphereSource.h>

using namespace std;

typedef unsigned int uint;

const int MAXN = 100;

class Point
{
public:
    double x, y, z;
    double s;
public:
    Point (double x, double y, double z, double s) : x(x), y(y), z(z), s(s) { };
};

void createSphere (vtkSmartPointer<vtkAppendPolyData> appendFilter, const Point p)
{
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetCenter(p.x,p.y,p.z);
    sphereSource->SetRadius(0.15);
    sphereSource->SetThetaResolution(15.0);
    sphereSource->SetPhiResolution(15.0);
    sphereSource->Update();

    // Color the points of the sphere with the same scalar of the point
    int numPtsSphere = sphereSource->GetOutput()->GetPoints()->GetNumberOfPoints();
    vtkSmartPointer<vtkDoubleArray> scalars = vtkSmartPointer<vtkDoubleArray>::New();
    scalars->SetNumberOfValues(numPtsSphere);
    for( int j = 0; j < numPtsSphere; j++ )
        scalars->SetValue(j,p.s);
    sphereSource->GetOutput()->GetPointData()->SetScalars(scalars);
    sphereSource->Update();

    // Append the sphere to the filter
    appendFilter->AddInputConnection(sphereSource->GetOutputPort());
    appendFilter->Update();
}

void processFile (int id)
{
    stringstream ss, ss2;
    ss << "../files/iter" << id << ".vtp";
    ss2 << "../files_mod/iter" << id << ".vtp";

    // Get all data from the file
    vtkXMLPolyDataReader* reader = vtkXMLPolyDataReader::New();
    reader->SetFileName(ss.str().c_str());
    reader->Update();
    vtkPolyData* polydata = reader->GetOutput();

    // Get the number of points the file contains
    vtkIdType NumPoints = polydata->GetNumberOfPoints();

    // Copy all the points
    vector<Point> points;
    double p[3], w;
    for(vtkIdType i = 0; i < NumPoints; i++)
    {
        polydata->GetPoint(i,p);
        // Caso o ponto possua escalar descomentar
        w = polydata->GetPointData()->GetScalars()->GetComponent(0,i);
        points.push_back(Point(p[0],p[1],p[2],w)); 
    }

    vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
    // For each point create a sphere
    for (vtkIdType i = 0; i < NumPoints; i++)
        createSphere(appendFilter,points[i]);
    // Write the file
    vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    writer->SetFileName(ss2.str().c_str());
    writer->SetInputConnection(appendFilter->GetOutputPort());
    writer->Write();
}

int main ()
{
    for (int i = 0; i < MAXN; i++)
        processFile(i);

    return 0;

}
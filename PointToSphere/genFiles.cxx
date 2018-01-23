#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
 
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkPolyData.h"
#include <vtkVersion.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkSmartPointer.h>
#include <vtkDoubleArray.h>

using namespace std;

typedef unsigned int uint;

const int MAXN = 100;

double f1 (double x)
{
    return sin(x);
}

double f2 (double x)
{
    return 5*sin(x);
}

void initVectors (double x[], double s1[], double s2[])
{
    double a = 0.0;
    double b = 2.0*M_PI;
    double dx = (b - a) / (double)MAXN;

    for (int i = 0; i < MAXN; i++)
    {
        x[i] = a + i*dx;
        s1[i] = f1(x[i]);
        s2[i] = f2(x[i]);
    }       
}

void genVTPFiles (const double s1[], const double s2[])
{
    // Create 2 points.
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0,0,0);
    points->InsertNextPoint(5,0,0);

    for (int i = 0; i < MAXN; i++)
    {
        // Store the points in a polydata strucuture
        vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
        polydata->SetPoints(points);

        vtkSmartPointer<vtkDoubleArray> weights = vtkSmartPointer<vtkDoubleArray>::New();
        weights->SetNumberOfValues(2);
        weights->SetValue(0,s1[i]);
        weights->SetValue(1,s2[i]);

        // Set the scalars values to the points
        polydata->GetPointData()->SetScalars(weights);

        // Set the filename
        stringstream ss;
        ss << "../files/iter" << i << ".vtp";

        // Write the file
        vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
        writer->SetFileName(ss.str().c_str());
        writer->SetInputData(polydata);
        writer->Write();
    }
}

int main ()
{
    // Initialize the scalars
    double x[MAXN];
    double scalars1[MAXN];
    double scalars2[MAXN];
    initVectors(x,scalars1,scalars2);

    // Generate the VTP files
    genVTPFiles(scalars1,scalars2);

    return 0;

}
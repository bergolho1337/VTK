#include <iostream>
#include <cmath>
#include <vector>

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
#include <vtkCylinderSource.h>
#include <vtkMath.h>
#include <vtkMatrix4x4.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkLine.h>

using namespace std;

class Point;
class Line;
class Edge;
class Node;

class Point
{
public:
    double x, y, z;
    double s;
public:
    Point (double x, double y, double z, double s) : x(x), y(y), z(z), s(s) { }
};

class Line
{
public:
    int to;
    int from;
public:
    Line (int to, int from) : to(to), from(from) { }
};

class Edge
{
public:
    int to;
    double weight;
public:
    Edge (int to, double weight) : to(to), weight(weight) { }

};

class Node
{
public:
    int id;
    double x, y, z;
    double scalar;
    vector<Edge> edges;
public:
    Node (double x, double y, double z, double s) : x(x), y(y), z(z), scalar(s) { };
};

double f1 (double x);
double f2 (double x);
double calcNorm (const Node u, const Node v);
void buildGraph (vector<Node> &g);
void printGraph (const vector<Node> g);

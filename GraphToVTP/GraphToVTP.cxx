/*
    Program that can build the structure of graph and export to a .vtp file making possible
the visualization on Paraview.  
    Each point of the graph can have it is own scalar over the time.

*/

#include <iostream>
#include "GraphToVTP.h"

using namespace std;

typedef unsigned int uint;

int NumPoints;
int NumLines;
const int MAXN = 100;

double f1 (double x) { return x; }
double f2 (double x) { return x*x; }

// Calculate the scalars from the nodes of the graph
void calcScalars (double s1[], double s2[], double x[])
{
    double a = 0.0;
    double b = 2.0;
    double dx = (b - a) / (double)MAXN;
    for (size_t i = 0; i < MAXN; i++)
    {
        x[i] = a + i*dx;
        s1[i] = f1(x[i]);
        s2[i] = f2(x[i]);
    }
}

// Calculate the Euclidean norm
double calcNorm (const Node u, const Node v)
{
    return sqrt( pow(u.x - v.x,2) + pow(u.y - v.y,2) + pow(u.z - v.z,2));
}

// Build the structure of the graph
void buildGraph (vector<Node> &g, const double s1, const double s2, const int id)
{
    // Insert nodes
    g.push_back(Node(0,0,0,s2));
    g.push_back(Node(1,0,0,s2));
    g.push_back(Node(2,0,0,s2));
    g.push_back(Node(2,1,0,s2));
    g.push_back(Node(2,-1,0,s2));
    g.push_back(Node(2,-2,0,s1));
    g.push_back(Node(2,2,0,s1));

    // Insert edges
    g[0].edges.push_back(Edge(1,calcNorm(g[0],g[1])));
    g[1].edges.push_back(Edge(2,calcNorm(g[1],g[2])));
    g[2].edges.push_back(Edge(3,calcNorm(g[2],g[3])));
    g[3].edges.push_back(Edge(6,calcNorm(g[3],g[6])));
    g[2].edges.push_back(Edge(4,calcNorm(g[2],g[4])));
    g[4].edges.push_back(Edge(5,calcNorm(g[4],g[5])));

    NumPoints = 7;
    NumLines = 6;
}

// Write the a .vtp file from the structure of the graph
void writeVTP (const vector<Node> g, const int id)
{
    // Set the filename
    stringstream ss;
    ss << "../files/iter" << id << ".vtp";

    // Insert the points
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    for (size_t i = 0; i < g.size(); i++)
        points->InsertNextPoint(g[i].x,g[i].y,g[i].z);
    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(points);

    // Set the scalars to the points
    vtkSmartPointer<vtkDoubleArray> weights = vtkSmartPointer<vtkDoubleArray>::New();
    weights->SetNumberOfValues(g.size());
    for (size_t i = 0; i < g.size(); i++)
        weights->SetValue(i,g[i].scalar);

    polydata->GetPointData()->SetScalars(weights);

    // Insert the lines using the edges of the graph
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
    for (size_t i = 0; i < g.size(); i++)
    {
        for (size_t j = 0; j < g[i].edges.size(); j++)
        {
            vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
            line->GetPointIds()->SetId(0,i);  
            line->GetPointIds()->SetId(1,g[i].edges[j].to);
            lines->InsertNextCell(line);
        }
    }
    polydata->SetLines(lines);

    // Write the polydata to a file
    vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    writer->SetFileName(ss.str().c_str());
    writer->SetInputData(polydata);
    writer->Write();

}

// Print the graph structure
void printGraph (const vector<Node> g)
{
    for (size_t i = 0; i < g.size(); i++)
    {
        cout << "|| " << i <<  " " << g[i].scalar << " ||";
        for (size_t j = 0; j < g[i].edges.size(); j++)
            cout << " --> || " << g[i].edges[j].to << " " << g[i].edges[j].weight << " ||";
        cout << endl;
    }
}

// Create a Sphere object from a Point of the graph and append it to Polydata filter
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

// Create a Cylinder object from a Point of the graph and append it to Polydata filter
void createCylinder (vtkSmartPointer<vtkAppendPolyData> &appendFilter, const vector<Point> points, \
                        const Line line)
{
    //Create an cylinder.
    // Cylinder height vector is (0,1,0).
    // Cylinder center is in the middle of the cylinder
    vtkSmartPointer<vtkCylinderSource> cylinderSource =
        vtkSmartPointer<vtkCylinderSource>::New();
    cylinderSource->SetRadius(0.1);
    cylinderSource->SetResolution(15);
    cylinderSource->Update();

    // Get coordinate of the points of the line
    int from, to;
    double startPoint[3], endPoint[3];
    from = line.from; to = line.to;
    startPoint[0] = points[from].x; startPoint[1] = points[from].y; startPoint[2] = points[from].z;
    endPoint[0] = points[to].x; endPoint[1] = points[to].y; endPoint[2] = points[to].z;

    // Color the points of the cylinder with the same scalar of the first point
    int numPtsCylinder = cylinderSource->GetOutput()->GetPoints()->GetNumberOfPoints();
    vtkSmartPointer<vtkDoubleArray> scalars = vtkSmartPointer<vtkDoubleArray>::New();
    scalars->SetNumberOfValues( numPtsCylinder );
    for( int j = 0; j < numPtsCylinder; j++ )
        scalars->SetValue(j,points[from].s);
    cylinderSource->GetOutput()->GetPointData()->SetScalars(scalars);
    cylinderSource->Update();

    // Compute a basis
    double normalizedX[3];
    double normalizedY[3];
    double normalizedZ[3];

    // The X axis is a vector from start to end
    vtkMath::Subtract(endPoint, startPoint, normalizedX);
    double length = vtkMath::Norm(normalizedX);
    vtkMath::Normalize(normalizedX);

    // The Z axis is an arbitrary vector cross X
    double arbitrary[3];
    arbitrary[0] = vtkMath::Random(-10,10);
    arbitrary[1] = vtkMath::Random(-10,10);
    arbitrary[2] = vtkMath::Random(-10,10);
    vtkMath::Cross(normalizedX, arbitrary, normalizedZ);
    vtkMath::Normalize(normalizedZ);

    // The Y axis is Z cross X
    vtkMath::Cross(normalizedZ, normalizedX, normalizedY);
    vtkSmartPointer<vtkMatrix4x4> matrix =
        vtkSmartPointer<vtkMatrix4x4>::New();

    // Create the direction cosine matrix
    matrix->Identity();
    for (unsigned int i = 0; i < 3; i++)
    {
        matrix->SetElement(i, 0, normalizedX[i]);
        matrix->SetElement(i, 1, normalizedY[i]);
        matrix->SetElement(i, 2, normalizedZ[i]);
    }

    // Apply the transforms
    vtkSmartPointer<vtkTransform> transform =
        vtkSmartPointer<vtkTransform>::New();
    transform->Translate(startPoint);   // translate to starting point
    transform->Concatenate(matrix);     // apply direction cosines
    transform->RotateZ(-90.0);          // align cylinder to x axis
    transform->Scale(1.0, length, 1.0); // scale along the height vector
    transform->Translate(0, .5, 0);     // translate to start of cylinder

    // Transform the polydata
    vtkSmartPointer<vtkTransformPolyDataFilter> transformPD =
        vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformPD->SetTransform(transform);
    transformPD->SetInputConnection(cylinderSource->GetOutputPort());

    // Append the cylinder to the filter
    appendFilter->AddInputConnection(transformPD->GetOutputPort());
    appendFilter->Update();
    
}

// Read a graph from .vtp file and convert it to a new structure where each point is a sphere 
// and each line is a cylinder
void convertGraph (const int id)
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

    // Copy all the points to a vector
    vector<Point> points;
    double p[3], w;
    for(vtkIdType i = 0; i < NumPoints; i++)
    {
        polydata->GetPoint(i,p);
        w = polydata->GetPointData()->GetScalars()->GetComponent(0,i);
        points.push_back(Point(p[0],p[1],p[2],w)); 
    }
    // Copy all the lines to a vector
    vector<Line> lines;
    vtkCellArray* LinesCells = polydata->GetLines();
    vtkIdType npts;
    vtkIdType *pts;

    while (LinesCells->GetNextCell(npts, pts))
    {
        Line line(pts[0],pts[1]);
        lines.push_back(line);
    }

    // Build a polydata from the graph structure
    vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
    // For each point create a sphere
    for (vtkIdType i = 0; i < NumPoints; i++)
        createSphere(appendFilter,points[i]);
    // For each line create a cylinder
    for (vtkIdType i = 0; i < NumLines; i++)
        createCylinder(appendFilter,points,lines[i]);
    
    // Write the polydata to a file
    vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    writer->SetFileName(ss2.str().c_str());
    writer->SetInputConnection(appendFilter->GetOutputPort());
    writer->Write();
}

// MAIN FUNCTION
int main ()
{
    double s1[MAXN], s2[MAXN], x[MAXN];
    calcScalars(s1,s2,x);

    for (size_t i = 0; i < MAXN; i++)
    {
        vector<Node> g;
        buildGraph(g,s1[i],s2[i],i);
        writeVTP(g,i);
        //printGraph(g);
    }

    for (size_t i = 0; i < MAXN; i++)
        convertGraph(i);

    return 0;

}
#include <vtkQuadric.h>
#include <vtkSampleFunction.h>
#include <vtkContourFilter.h>
#include <vtkOutlineFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkPolyDataWriter.h>
#include <vtkSTLWriter.h>

int main(int, char *argv[])
{
  // General form
  // F(x,y,z) = a0*x^2 + a1*y^2 + a2*z^2 + a3*x*y + a4*y*z + a5*x*z + a6*x + a7*y + a8*z + a9

  // Paraboloid
  // F(x,y,z) = 1*x^2 + 1*y^2 - 1 --> Paraboloid
  
  // Ellipsoid
  // F(x,y,z) = 1*x^2 + 0.5*y^2 + 0.7*z^2 - 1 --> Ellipsoid
  
  // Create the quadric function definition
  vtkSmartPointer<vtkQuadric> quadric = vtkSmartPointer<vtkQuadric>::New();

  // Paraboloid
  quadric->SetCoefficients(1,1,0,0,0,0,0,0,-1,0);
  
  // Ellipsoid
  //quadric->SetCoefficients(1,0.5,0.7,0,0,0,0,0,-1,0);
 
  // Sample the quadric function
  vtkSmartPointer<vtkSampleFunction> sample = vtkSmartPointer<vtkSampleFunction>::New();
  sample->SetSampleDimensions(200,200,200);
  sample->SetImplicitFunction(quadric);
  //double xmin = -2, xmax=2, ymin=-2, ymax=2, zmin=-1, zmax=1;
  double xmin = -10, xmax=11, ymin=-10, ymax=10, zmin=-10, zmax=10;
  sample->SetModelBounds(xmin, xmax, ymin, ymax, zmin, zmax);

  //create the 0 isosurface (domain)
  vtkSmartPointer<vtkContourFilter> contours = vtkSmartPointer<vtkContourFilter>::New();
  contours->SetInputConnection(sample->GetOutputPort());
  contours->GenerateValues(1,1,1);

  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(contours->GetOutputPort());
  mapper->Update();   

  // Get the reference to the Polydata
  vtkPolyData *polydata = mapper->GetInput();	

  vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
  writer->SetFileName("outputs/quadratic.vtk");
  writer->SetInputData(polydata);
  writer->Write();

  vtkSmartPointer<vtkSTLWriter> stlWriter = vtkSmartPointer<vtkSTLWriter>::New();
  stlWriter->SetFileName("outputs/quadratic.stl");
  stlWriter->SetInputData(polydata);
  stlWriter->Write();

  return EXIT_SUCCESS;
}

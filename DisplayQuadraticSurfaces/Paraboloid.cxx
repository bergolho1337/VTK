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

int main(int, char *argv[])
{
  // create the quadric function definition
  vtkSmartPointer<vtkQuadric> quadric = vtkSmartPointer<vtkQuadric>::New();
  quadric->SetCoefficients(1,1,0,0,0,0,0,0,-1,0);
  
  // F(x,y,z) = a0*x^2 + a1*y^2 + a2*z^2 + a3*x*y + a4*y*z + a5*x*z + a6*x + a7*y + a8*z + a9
  // F(x,y,z) = 1*x^2 + 1*y^2
 
  // Sample the quadric function
  vtkSmartPointer<vtkSampleFunction> sample = vtkSmartPointer<vtkSampleFunction>::New();
  sample->SetSampleDimensions(50,50,50);
  sample->SetImplicitFunction(quadric);
  double xmin = -2, xmax=2, ymin=-2, ymax=2, zmin=-2, zmax=2;
  //double xmin = -10, xmax=11, ymin=-10, ymax=10, zmin=-10, zmax=10;
  sample->SetModelBounds(xmin, xmax, ymin, ymax, zmin, zmax);

  //create the 0 isosurface (domain)
  vtkSmartPointer<vtkContourFilter> contours = vtkSmartPointer<vtkContourFilter>::New();
  contours->SetInputConnection(sample->GetOutputPort());
  contours->GenerateValues(1,1,1);

  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(contours->GetOutputPort());  

  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);

  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);
 
  renderer->AddActor(actor);
  renderer->SetBackground(1,1,1);

  renderWindow->Render();
  renderWindowInteractor->Start();	

  vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
  writer->SetFileName( "paraboloid.vtk" );
  writer->SetInputData( mapper->GetInput() );
  writer->Write();

  return EXIT_SUCCESS;
}

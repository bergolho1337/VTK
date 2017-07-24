#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include "vtkCellArray.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
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

int main(int, char *argv[])
{
  // Create a sphere
  vtkSmartPointer<vtkCylinderSource> cylinderSource =
    vtkSmartPointer<vtkCylinderSource>::New();
  cylinderSource->SetCenter(0.0, 0.0, 0.0);
  cylinderSource->SetRadius(5.0);
  cylinderSource->SetHeight(7.0);
  cylinderSource->SetResolution(100);
  cylinderSource->Update();

  // Color the points of the cylinder with the same scalar of the first point
  int numPtsCylinder = cylinderSource->GetOutput()->GetPoints()->GetNumberOfPoints();
  vtkSmartPointer<vtkFloatArray> scalars =
      vtkSmartPointer<vtkFloatArray>::New();
  scalars->SetNumberOfValues( numPtsCylinder );
  for( int j = 0; j < numPtsCylinder; j++ )
      scalars->SetValue(j,j*0.0025);
  cylinderSource->GetOutput()->GetPointData()->SetScalars(scalars);
  cylinderSource->Update();
 
  // Create a mapper and actor
  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(cylinderSource->GetOutputPort());
  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
 
  //Create a renderer, render window, and interactor
  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);
 
  // Add the actor to the scene
  renderer->AddActor(actor);
  renderer->SetBackground(.1, .3,.2); // Background color dark green
 
  // Render and interact
  renderWindow->SetWindowName(argv[0]);
  renderWindow->Render();
  renderWindowInteractor->Start();
 
  return EXIT_SUCCESS;
}

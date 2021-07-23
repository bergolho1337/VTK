#include <vtkXMLPolyDataReader.h>
#include <vtkXMLUnstructuredGridReader.h>
#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkSphereSource.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkUnstructuredGrid.h>
#include <vtkIdList.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkDijkstraGraphGeodesicPath.h>

int main (int argc, char **argv)
{ 
  if (argc-1 != 3)
  {
      printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
      printf("Usage:> %s <input_filename> <source_index> <target_index>\n",argv[0]);
      printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
      exit(EXIT_FAILURE);   
  }

  std::string filename = argv[1];
  std::string extension = filename.substr(filename.size()-3,3);
  uint32_t source = atoi(argv[2]);
  uint32_t target = atoi(argv[3]);

  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  vtkPolyData* polydata_grid;
  vtkUnstructuredGrid *unstructured_grid;

  if (extension == "stl")
  {
    vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName("inputs/cadim_full_mesh_cardiax.stl");
    reader->Update();

    mapper->SetInputConnection(reader->GetOutputPort());
    mapper->Update();

    polydata_grid = mapper->GetInput();
  }
  else if (extension == "vtp")
  {
    vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
    reader->SetFileName(filename.c_str());
    reader->Update();

    mapper->SetInputConnection(reader->GetOutputPort());
    mapper->Update();

    polydata_grid = mapper->GetInput();

  }
  else if (extension == "vtu")
  {
    vtkSmartPointer<vtkXMLUnstructuredGridReader> reader = vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
    reader->SetFileName(filename.c_str());
    reader->Update();

    unstructured_grid = reader->GetOutput();
  }
  else
  {
    fprintf(stderr,"[-] ERROR! Invalid file format '%s'! Only 'stl' and 'vtp' format are acceptable!\n",extension.c_str());
    exit(EXIT_FAILURE);
  }

  vtkSmartPointer<vtkDijkstraGraphGeodesicPath> dijkstra = vtkSmartPointer<vtkDijkstraGraphGeodesicPath>::New();
  if (extension == "vtk" || extension == "vtp")
    dijkstra->SetInputData(polydata_grid);
  else if (extension == "vtu")
    dijkstra->SetInputData(unstructured_grid);

  dijkstra->SetStartVertex(source);
  dijkstra->SetEndVertex(target);
  dijkstra->Update();

  // Get the ids of the path
  printf("Geodesic path from point '%u' to '%u'\n",source,target);
  vtkSmartPointer<vtkIdList> ids = dijkstra->GetIdList();
  uint32_t num_points = ids->GetNumberOfIds();
  for (uint32_t i = 0; i < num_points; i++)
  {
    uint32_t id = ids->GetId(i);

    double pos[3];

    polydata_grid->GetPoint(id,pos);
    printf("%u - (%g %g %g)\n",id,pos[0],pos[1],pos[2]);
  }

/*
  // VISUALIZATION  
  // Create a mapper and actor
  vtkSmartPointer<vtkPolyDataMapper> pathMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  pathMapper->SetInputConnection(dijkstra->GetOutputPort());

  vtkSmartPointer<vtkActor> pathActor = vtkSmartPointer<vtkActor>::New();
  pathActor->SetMapper(pathMapper);
  pathActor->GetProperty()->SetColor(1,0,0); // Red
  pathActor->GetProperty()->SetLineWidth(4);

  // Create a mapper and actor
  //vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputData(polydata_grid);

  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  //Create a renderer, render window, and interactor
  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  //Add the actor to the scene
  renderer->AddActor(actor);
  renderer->AddActor(pathActor);
  renderer->SetBackground(.3, .6, .3); // Background color green

  //Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();
*/
  return EXIT_SUCCESS;
}
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
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkLine.h>

class Point
{
public:
  double pos[3];
public:
  Point (const double p[])
  {
    memcpy(this->pos,p,sizeof(double)*3);
  }
  void print ()
  {
    printf("pos=[%g %g %g]\n",pos[0],pos[1],pos[2]);
  }
};

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
    reader->SetFileName(filename.c_str());
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
  if (extension == "stl" || extension == "vtp")
    dijkstra->SetInputData(polydata_grid);
  else if (extension == "vtu")
    dijkstra->SetInputData(unstructured_grid);

  dijkstra->SetStartVertex(source);
  dijkstra->SetEndVertex(target);
  dijkstra->Update();

  // Get the ids of the path
  //printf("Geodesic path from point '%u' to '%u'\n",source,target);
  vtkSmartPointer<vtkIdList> ids = dijkstra->GetIdList();
  uint32_t num_points = ids->GetNumberOfIds();
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkCellArray> cell_array = vtkSmartPointer<vtkCellArray>::New();
  for (uint32_t i = 0; i < num_points; i++)
  {
    double pos[3];
    uint32_t id = ids->GetId(i);
    polydata_grid->GetPoint(id,pos);
    points->InsertNextPoint(pos[0],pos[1],pos[2]);

    if (i < num_points-1)
    {
      vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
      line->GetPointIds()->SetId(0,i);
      line->GetPointIds()->SetId(1,i+1);
      cell_array->InsertNextCell(line);
    }
  }
  vtkSmartPointer<vtkUnstructuredGrid> output_grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
  output_grid->SetPoints(points);
  output_grid->SetCells(VTK_LINE,cell_array);

  vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
  writer->SetFileName("outputs/pathway.vtu");
  writer->SetInputData(output_grid);
  writer->Write();

  return EXIT_SUCCESS;
}

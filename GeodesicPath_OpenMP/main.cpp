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
#include <omp.h>

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

void Hello (int &count) {
  int my_rank = omp_get_thread_num();
  int thread_count = omp_get_num_threads();

  printf("Hello world from thread %d of %d -- count = %d\n",my_rank,thread_count,count);  
  count++;
}

int main (int argc, char **argv)
{ 
  std::string filename = "inputs/cadim_full_mesh_cardiax.stl";
  std::string extension = filename.substr(filename.size()-3,3);
  int source = 35;
  //int target[4] = {320,1275,1530,1654};
  int target[1000];
  for (int k = 0; k < 1000; k++)
    target[k] = 1000+k;

  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  vtkPolyData* polydata_grid;
  vtkUnstructuredGrid *unstructured_grid;

  // 1) Read the surface
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

  omp_set_num_threads(4);
  #pragma omp parallel for
  for (int k = 0; k < 1000; k++) {

    // Build the 'vtkDijkstraGraphGeodesicPath' structure
    vtkSmartPointer<vtkDijkstraGraphGeodesicPath> dijkstra = vtkSmartPointer<vtkDijkstraGraphGeodesicPath>::New();
    if (extension == "stl" || extension == "vtp")
      dijkstra->SetInputData(polydata_grid);
    else if (extension == "vtu")
      dijkstra->SetInputData(unstructured_grid);

    int thread_id = omp_get_thread_num();

    // Set the 'EndVertex' accordindly to the current 'thread_id'
    dijkstra->SetStartVertex(source);
    dijkstra->SetEndVertex(target[k]);
    dijkstra->Update();

    // Get the ids of the path
    printf("[thread=%d] Geodesic path from point '%d' to '%d'\n",thread_id,source,target[k]);
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

    char output_file[50];
    sprintf(output_file,"outputs/pathway_%d.vtu",k);
    vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
    writer->SetFileName(output_file);
    writer->SetInputData(output_grid);
    writer->Write();
  }
  

/*
  int count = 0;
  int thread_count = 4;
  omp_set_num_threads(thread_count);
  #pragma omp parallel private(count)
  for (int i = 0; i < 5; i++)
    Hello(count);
  return EXIT_SUCCESS;
*/
}

#include <iostream>
#include <string>
#include <map>

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <vtkXMLUnstructuredGridReader.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkUnstructuredGrid.h>
#include <vtkHexahedron.h>
#include <vtkSphereSource.h>
#include <vtkAppendPolyData.h>
#include <vtkFloatArray.h>
#include <vtkCellData.h>
#include <vtkLine.h>

using namespace std;

class PointData
{
public:
  uint32_t id;
  double x, y, z;
  double scalar;
public:
  PointData (const uint32_t id, const double x, const double y, const double z)
  {
    this->id = id;
    this->x = x;
    this->y = y;
    this->z = z;
    this->scalar = 0.0;
  }
};

void write_pointdata_coordinates_to_pts (std::vector<PointData> pointdata)
{
  FILE *file = fopen("outputs/pointdata_coordinates.pts","w+");

  fprintf(file,"%u\n",pointdata.size());
  for (int i = 0; i < pointdata.size(); i++)
    fprintf(file,"%g %g %g\n",pointdata[i].x*1.0e-05,pointdata[i].y*1.0e-05,pointdata[i].z*1.0e-05);

  fclose(file);
}

void write_pointdata_scalars_to_txt (std::vector<PointData> pointdata)
{
  FILE *file = fopen("outputs/pointdata_scalars.txt","w+");

  fprintf(file,"%u\n",pointdata.size());
  for (uint32_t i = 0; i < pointdata.size(); i++)
    fprintf(file,"%g\n",pointdata[i].scalar);

  fclose(file);
}

void write_pointdata_to_vtk (std::vector<PointData> pointdata)
{
  FILE *file = fopen("outputs/pointdata.vtk","w+");

  fprintf(file,"# vtk DataFile Version 4.2\n");
  fprintf(file,"vtk output\n");
  fprintf(file,"ASCII\n");
  fprintf(file,"DATASET POLYDATA\n");
  fprintf(file,"POINTS %u float\n",pointdata.size());
  
  for (uint32_t i = 0; i < pointdata.size(); i++)
    fprintf(file,"%g %g %g\n",pointdata[i].x,pointdata[i].y,pointdata[i].z);
  
  fprintf(file,"VERTICES %u %u\n",pointdata.size(),pointdata.size()*2);
  for (uint32_t i = 0; i < pointdata.size(); i++)
    fprintf(file,"1 %u\n",i);

  fprintf(file,"POINT_DATA %u\n",pointdata.size());
  fprintf(file,"SCALARS Scalars_ float\n");
  fprintf(file,"LOOKUP_TABLE default\n");

  for (uint32_t i = 0; i < pointdata.size(); i++)
    fprintf(file,"%g\n",pointdata[i].scalar);

  fclose(file);
}

int main (int argc, char *argv[])
{
  
  // ------------------------------------------------------------------------------------------------------------------------------
  // READER
  vtkSmartPointer<vtkXMLUnstructuredGridReader> reader = vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
  reader->SetFileName("inputs/tissue_activation_time_map_pulse_it_0.vtu");
  reader->Update();

  vtkUnstructuredGrid *unstructured_grid = reader->GetOutput();

  // Read cells
  int num_cells = unstructured_grid->GetNumberOfCells();
  int num_points = unstructured_grid->GetNumberOfPoints();

  cout << "Number of points = " << num_points << endl;
  cout << "Number of cells = " << num_cells << endl;

  std::vector<PointData> pointdata;
  for (int i = 0; i < num_cells; i++)
  {
    //cout << "Cell " << i << endl;
    vtkCell *cell = unstructured_grid->GetCell(i);
    vtkHexahedron *hexahedron = dynamic_cast<vtkHexahedron*>(cell);

    double p0[3], p1[3], p3[3], p4[3];
    hexahedron->GetPoints()->GetPoint(0,p0);
    hexahedron->GetPoints()->GetPoint(1,p1);
    hexahedron->GetPoints()->GetPoint(3,p3);
    hexahedron->GetPoints()->GetPoint(4,p4);

    double center[3];
    center[0] = p0[0] + (p1[0] - p0[0])/2.0;
    center[1] = p0[1] + (p3[1] - p0[1])/2.0;
    center[2] = p0[2] + (p4[2] - p0[2])/2.0;

    PointData p(i,center[0],center[1],center[2]);
    pointdata.push_back(p);
  }

  // Read cells scalar values
  string array_name = "Scalars_";
  vtkSmartPointer<vtkFloatArray> array = vtkFloatArray::SafeDownCast(unstructured_grid->GetCellData()->GetArray(array_name.c_str()));

  if(array)
  {
      // Pass through each cell extracting the scalar value
      for(int i = 0; i < num_cells; i++)
      {
        double value = array->GetValue(i);
        pointdata[i].scalar = value;
      }
  }

  write_pointdata_coordinates_to_pts(pointdata);
  write_pointdata_scalars_to_txt(pointdata);
  write_pointdata_to_vtk(pointdata);

  return 0;

}

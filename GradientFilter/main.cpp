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
#include <vtkVertex.h>
#include <vtkGradientFilter.h>
#include <vtkAssignAttribute.h>
#include <vtkArrowSource.h>
#include <vtkGlyph3D.h>
#include <vtkPolyDataMapper.h>

using namespace std;

#define N 300

class Point
{
public:
  float pos[3];
public:
  Point (const float p[])
  {
    memcpy(this->pos,p,sizeof(float)*3);
  }
  void print ()
  {
    printf("pos=[%g %g %g]\n",pos[0],pos[1],pos[2]);
  }
};

class Cell
{
public:
  float lat;
  float v[3];
  float center[3];
public:
  Cell (const float lat, const float v[], const float c[])
  {
    this->lat = lat;
    memcpy(this->v,v,sizeof(float)*3);
    memcpy(this->center,c,sizeof(float)*3);
  }
  void print ()
  {
    printf("center=[%g %g %g], LAT=%g, v=[%g %g %g]\n",center[0],center[1],center[2],\
                                                      lat,v[0],v[1],v[2]);
  }
};

void normalize (float v[])
{
  float norm = sqrt(pow(v[0],2)+pow(v[1],2)+pow(v[2],2));
  v[0] /= norm;
  v[1] /= norm;
  v[2] /= norm;
}

Cell get_closest_cell (vector<Cell> cells, const float pos[])
{
  uint32_t closest_id = 0;
  double closest_dist = __DBL_MAX__;
  for (uint32_t i = 0; i < cells.size(); i++)
  {
    double dist = sqrt(pow(pos[0]-cells[i].center[0],2)+pow(pos[1]-cells[i].center[1],2)+pow(pos[2]-cells[i].center[2],2));
    if (dist < closest_dist)
    {
      closest_dist = dist;
      closest_id = i;
    }
  }
  return cells[closest_id];
}

void read_pmjs (vector<Point> &pmjs, const char filename[])
{
  char str[500];
  FILE *file = fopen(filename,"r");
  while (fscanf(file,"%s",str) != EOF)
        if (strcmp(str,"POINTS") == 0) break;
    
    uint32_t num_points;
    fscanf(file,"%u %s",&num_points,str);
    for (uint32_t i = 0; i < num_points; i++)
    {
        float pos[3];
        fscanf(file,"%f %f %f",&pos[0],&pos[1],&pos[2]);
        Point pmj(pos);
        pmjs.push_back(pmj);
    }
  fclose(file);
}

void write_pmjs_path (vector<Cell> cells, Point pmj, const uint32_t id)
{
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkCellArray> cell_array = vtkSmartPointer<vtkCellArray>::New();
  vtkSmartPointer<vtkFloatArray> cells_dataset = vtkSmartPointer<vtkFloatArray>::New();

  float *target_pos = pmj.pos;
  char output_filename[500];
  sprintf(output_filename,"outputs/path_%u.vtu",id+1);
  printf("[PMJ %u] %g %g %g --> Building path ...\n",id+1,target_pos[0],target_pos[1],target_pos[2]);
  
  for (uint32_t k = 0; k < N; k++)
  {
    Cell closest_cell = get_closest_cell(cells,target_pos);
    points->InsertNextPoint(closest_cell.center[0],closest_cell.center[1],closest_cell.center[2]);
    if (k < N-1)
    {
      vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
      line->GetPointIds()->SetId(0,k);
      line->GetPointIds()->SetId(1,k+1);
      cell_array->InsertNextCell(line);
      cells_dataset->InsertNextValue(k);
    }

    target_pos[0] += closest_cell.v[0]*100;
    target_pos[1] += closest_cell.v[1]*100;
    target_pos[2] += closest_cell.v[2]*100;

  }
  vtkSmartPointer<vtkUnstructuredGrid> path_grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
  path_grid->SetPoints(points);
  path_grid->SetCells(VTK_LINE,cell_array);
  path_grid->GetCellData()->SetScalars(cells_dataset);

  vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
  writer->SetFileName(output_filename);
  writer->SetInputData(path_grid);
  writer->Write();
}

int main (int argc, char *argv[])
{
    if (argc-1 != 1)
    {
        printf("Usage:> %s <activation_time_map>\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    vtkSmartPointer<vtkXMLUnstructuredGridReader> reader = vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
    reader->SetFileName(argv[1]);
    reader->Update();

    vtkUnstructuredGrid *unstructured_grid = reader->GetOutput();

    uint32_t num_cells = unstructured_grid->GetNumberOfCells();
    uint32_t num_points = unstructured_grid->GetNumberOfPoints();
    
    cout << "Number of points = " << num_points << endl;
    cout << "Number of cells = " << num_cells << endl;

    vtkSmartPointer<vtkGradientFilter> gradients = vtkSmartPointer<vtkGradientFilter>::New();
    gradients->SetInputConnection(reader->GetOutputPort());
    gradients->Update();

    vtkSmartPointer<vtkAssignAttribute> vectors = vtkSmartPointer<vtkAssignAttribute>::New();
    vectors->SetInputConnection(gradients->GetOutputPort());
    vectors->Assign("Gradients", vtkDataSetAttributes::VECTORS, vtkAssignAttribute::POINT_DATA);
    //vectors->Update();

    vtkUnstructuredGrid *glyph_grid = gradients->GetUnstructuredGridOutput();
    vtkSmartPointer<vtkFloatArray> array_1 = vtkFloatArray::SafeDownCast(glyph_grid->GetCellData()->GetArray("Scalars_"));
    vtkSmartPointer<vtkFloatArray> array_2 = vtkFloatArray::SafeDownCast(glyph_grid->GetCellData()->GetArray("Gradients"));
    std::vector<Cell> cells;
    if(array_1 && array_2)
    {
      for(uint32_t i = 0; i < num_cells; i++)
      {
        float lat;
        float v[3];
        float c[3];
        lat = array_1->GetValue(i);
        array_2->GetTypedTuple(i,v);
        normalize(v);
        
        double p0[3], p1[3], p2[3], p4[3];
        vtkCell *cell = glyph_grid->GetCell(i);
        vtkHexahedron *hexahedron = dynamic_cast<vtkHexahedron*>(cell);
        hexahedron->GetPoints()->GetPoint(0,p0);
        hexahedron->GetPoints()->GetPoint(1,p1);
        hexahedron->GetPoints()->GetPoint(2,p2);
        hexahedron->GetPoints()->GetPoint(4,p4);
        c[0] = p0[0] + (p1[0]-p0[0])/2.0;
        c[1] = p0[1] + (p2[1]-p0[1])/2.0;
        c[2] = p0[2] + (p4[2]-p0[2])/2.0;

        Cell new_cell(lat,v,c);
        cells.push_back(new_cell);
      }
    }

    vector<Point> pmjs;
    read_pmjs(pmjs,"inputs/canine_mesh_active_pmj_locations.vtk");
  
    for (uint32_t i = 0; i < 10; i++)
    {
      write_pmjs_path(cells,pmjs[i],i+1);
    }
    
    return 0;
}
#include <iostream>
#include <string>
#include <map>

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <vtkXMLUnstructuredGridReader.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkXMLPolyDataReader.h>
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
#include <vtkPointData.h>

using namespace std;

double f1 (const double x) { return x; }
double f2 (const double x) { return x*x; }


int main (int argc, char *argv[])
{
  // -------------------------------------------------------------------------------------------------------------------------------
  // WRITER
  // Create all the points
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  points->InsertNextPoint(0,1,0);
  points->InsertNextPoint(1,1,0);
  points->InsertNextPoint(2,1,0);
  points->InsertNextPoint(3,2,0);
  points->InsertNextPoint(3,0,0);

  // Create all the cells
  vtkSmartPointer<vtkLine> line_1  = vtkSmartPointer<vtkLine>::New();
  line_1->GetPointIds()->SetId(0,0);
  line_1->GetPointIds()->SetId(1,1);

  vtkSmartPointer<vtkLine> line_2  = vtkSmartPointer<vtkLine>::New();
  line_2->GetPointIds()->SetId(0,1);
  line_2->GetPointIds()->SetId(1,2);

  vtkSmartPointer<vtkLine> line_3  = vtkSmartPointer<vtkLine>::New();
  line_3->GetPointIds()->SetId(0,2);
  line_3->GetPointIds()->SetId(1,3);

  vtkSmartPointer<vtkLine> line_4  = vtkSmartPointer<vtkLine>::New();
  line_4->GetPointIds()->SetId(0,2);
  line_4->GetPointIds()->SetId(1,4);

  // Store the lines into a cell array
  vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
  lines->InsertNextCell(line_1);
  lines->InsertNextCell(line_2);
  lines->InsertNextCell(line_3);
  lines->InsertNextCell(line_4);

  // Set the cell values into an array
  vtkSmartPointer<vtkFloatArray> values_dataset_1 = vtkSmartPointer<vtkFloatArray>::New();
  vtkSmartPointer<vtkFloatArray> values_dataset_2 = vtkSmartPointer<vtkFloatArray>::New();
  for (int i = 0; i < 5; i++)
  {
    values_dataset_1->InsertNextValue(f1(i));
    values_dataset_2->InsertNextValue(f2(i-3));
  }

  // Build the vtkUnstructuredGrid
  vtkSmartPointer<vtkPolyData> polydata_grid = vtkSmartPointer<vtkPolyData>::New();
  polydata_grid->SetPoints(points);
  polydata_grid->SetLines(lines);
  polydata_grid->GetPointData()->SetScalars(values_dataset_1);

  // Write the vtkUnstructuredGrid into a file
  vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetFileName("outputs/dataset_1.vtp");
  writer->SetInputData(polydata_grid);
  writer->Write();

  polydata_grid->GetPointData()->SetScalars(values_dataset_2);
  writer->SetFileName("outputs/dataset_2.vtp");
  writer->SetInputData(polydata_grid);
  writer->Write();

  // ------------------------------------------------------------------------------------------------------------------------------
  // READER
  vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  reader->SetFileName("outputs/dataset_1.vtp");
  reader->Update();

  vtkPolyData* polydata_grid_2 = reader->GetOutput();

  // Read cells
  int num_cells = polydata_grid_2->GetNumberOfCells();
  int num_points = polydata_grid_2->GetNumberOfPoints();

  cout << "Number of points = " << num_points << endl;
  cout << "Number of cells = " << num_cells << endl;

  // Read the points
  for (int i = 0; i < num_points; i++)
  {
    double pos[3];

    polydata_grid_2->GetPoint(i,pos);

    cout << "Point " << i << " = (" << pos[0] << "," << pos[1] << "," << pos[2] << ")" << endl;
  }

  // Read the cells
  for (int i = 0; i < num_cells; i++)
  {
    cout << "Cell " << i << endl;

    vtkCell *cell = polydata_grid_2->GetCell(i);

    vtkLine *line = dynamic_cast<vtkLine*>(cell);

    int src = line->GetPointIds()->GetId(0);
    int dest = line->GetPointIds()->GetId(1);

    cout << "\t " << src << " --> " << dest << endl;
  }

  // Read points scalar values
  string array_name = "Scalars_";

  vtkSmartPointer<vtkFloatArray> array = vtkFloatArray::SafeDownCast(polydata_grid_2->GetPointData()->GetArray(array_name.c_str()));

  if(array)
  {
      // Pass through each cell extracting the scalar value
      for(int i = 0; i < num_points; i++)
      {
          double value = array->GetValue(i);

          cout << "Point " << i << " -- Scalar value = " << value << endl;
      }
  }

  return 0;

}

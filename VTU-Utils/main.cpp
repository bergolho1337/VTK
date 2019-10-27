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

using namespace std;

double f1 (const double x) { return x; }
double f2 (const double x) { return x*x; }


int main (int argc, char *argv[])
{
  // -------------------------------------------------------------------------------------------------------------------------------
  // WRITER
  // Create all the points
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  points->InsertNextPoint(0,0,0);
  points->InsertNextPoint(1,0,0);
  points->InsertNextPoint(2,0,0);
  points->InsertNextPoint(3,0,0);
  points->InsertNextPoint(0,1,0);
  points->InsertNextPoint(1,1,0);
  points->InsertNextPoint(2,1,0);
  points->InsertNextPoint(3,1,0);
  points->InsertNextPoint(0,2,0);
  points->InsertNextPoint(1,2,0);
  points->InsertNextPoint(2,2,0);
  points->InsertNextPoint(3,2,0);
  points->InsertNextPoint(0,3,0);
  points->InsertNextPoint(1,3,0);
  points->InsertNextPoint(2,3,0);
  points->InsertNextPoint(3,3,0);
  points->InsertNextPoint(0,0,1);
  points->InsertNextPoint(1,0,1);
  points->InsertNextPoint(2,0,1);
  points->InsertNextPoint(3,0,1);
  points->InsertNextPoint(0,1,1);
  points->InsertNextPoint(1,1,1);
  points->InsertNextPoint(2,1,1);
  points->InsertNextPoint(3,1,1);
  points->InsertNextPoint(0,2,1);
  points->InsertNextPoint(1,2,1);
  points->InsertNextPoint(2,2,1);
  points->InsertNextPoint(3,2,1);
  points->InsertNextPoint(0,3,1);
  points->InsertNextPoint(1,3,1);
  points->InsertNextPoint(2,3,1);
  points->InsertNextPoint(3,3,1);

  // Create all the cells
  int offset = 16;
  vtkSmartPointer<vtkHexahedron> hexahedron_1 = vtkSmartPointer<vtkHexahedron>::New();
  hexahedron_1->GetPointIds()->SetId(0,0);
  hexahedron_1->GetPointIds()->SetId(1,4);
  hexahedron_1->GetPointIds()->SetId(2,offset+4);
  hexahedron_1->GetPointIds()->SetId(3,offset+0);
  hexahedron_1->GetPointIds()->SetId(4,1);
  hexahedron_1->GetPointIds()->SetId(5,5);
  hexahedron_1->GetPointIds()->SetId(6,offset+5);
  hexahedron_1->GetPointIds()->SetId(7,offset+1);

  vtkSmartPointer<vtkHexahedron> hexahedron_2 = vtkSmartPointer<vtkHexahedron>::New();
  hexahedron_2->GetPointIds()->SetId(0,1);
  hexahedron_2->GetPointIds()->SetId(1,5);
  hexahedron_2->GetPointIds()->SetId(2,offset+5);
  hexahedron_2->GetPointIds()->SetId(3,offset+1);
  hexahedron_2->GetPointIds()->SetId(4,2);
  hexahedron_2->GetPointIds()->SetId(5,6);
  hexahedron_2->GetPointIds()->SetId(6,offset+6);
  hexahedron_2->GetPointIds()->SetId(7,offset+2);

  vtkSmartPointer<vtkHexahedron> hexahedron_3 = vtkSmartPointer<vtkHexahedron>::New();
  hexahedron_3->GetPointIds()->SetId(0,2);
  hexahedron_3->GetPointIds()->SetId(1,3);
  hexahedron_3->GetPointIds()->SetId(2,offset+3);
  hexahedron_3->GetPointIds()->SetId(3,offset+2);
  hexahedron_3->GetPointIds()->SetId(4,6);
  hexahedron_3->GetPointIds()->SetId(5,7);
  hexahedron_3->GetPointIds()->SetId(6,offset+7);
  hexahedron_3->GetPointIds()->SetId(7,offset+6);

  vtkSmartPointer<vtkHexahedron> hexahedron_4 = vtkSmartPointer<vtkHexahedron>::New();
  hexahedron_4->GetPointIds()->SetId(0,4);
  hexahedron_4->GetPointIds()->SetId(1,5);
  hexahedron_4->GetPointIds()->SetId(2,offset+5);
  hexahedron_4->GetPointIds()->SetId(3,offset+4);
  hexahedron_4->GetPointIds()->SetId(4,8);
  hexahedron_4->GetPointIds()->SetId(5,9);
  hexahedron_4->GetPointIds()->SetId(6,offset+9);
  hexahedron_4->GetPointIds()->SetId(7,offset+8);

  vtkSmartPointer<vtkHexahedron> hexahedron_5 = vtkSmartPointer<vtkHexahedron>::New();
  hexahedron_5->GetPointIds()->SetId(0,5);
  hexahedron_5->GetPointIds()->SetId(1,6);
  hexahedron_5->GetPointIds()->SetId(2,offset+6);
  hexahedron_5->GetPointIds()->SetId(3,offset+5);
  hexahedron_5->GetPointIds()->SetId(4,9);
  hexahedron_5->GetPointIds()->SetId(5,10);
  hexahedron_5->GetPointIds()->SetId(6,offset+10);
  hexahedron_5->GetPointIds()->SetId(7,offset+9);

  vtkSmartPointer<vtkHexahedron> hexahedron_6 = vtkSmartPointer<vtkHexahedron>::New();
  hexahedron_6->GetPointIds()->SetId(0,6);
  hexahedron_6->GetPointIds()->SetId(1,7);
  hexahedron_6->GetPointIds()->SetId(2,offset+7);
  hexahedron_6->GetPointIds()->SetId(3,offset+6);
  hexahedron_6->GetPointIds()->SetId(4,10);
  hexahedron_6->GetPointIds()->SetId(5,11);
  hexahedron_6->GetPointIds()->SetId(6,offset+11);
  hexahedron_6->GetPointIds()->SetId(7,offset+10);

  vtkSmartPointer<vtkHexahedron> hexahedron_7 = vtkSmartPointer<vtkHexahedron>::New();
  hexahedron_7->GetPointIds()->SetId(0,8);
  hexahedron_7->GetPointIds()->SetId(1,9);
  hexahedron_7->GetPointIds()->SetId(2,offset+9);
  hexahedron_7->GetPointIds()->SetId(3,offset+8);
  hexahedron_7->GetPointIds()->SetId(4,12);
  hexahedron_7->GetPointIds()->SetId(5,13);
  hexahedron_7->GetPointIds()->SetId(6,offset+13);
  hexahedron_7->GetPointIds()->SetId(7,offset+12);

  vtkSmartPointer<vtkHexahedron> hexahedron_8 = vtkSmartPointer<vtkHexahedron>::New();
  hexahedron_8->GetPointIds()->SetId(0,9);
  hexahedron_8->GetPointIds()->SetId(1,10);
  hexahedron_8->GetPointIds()->SetId(2,offset+10);
  hexahedron_8->GetPointIds()->SetId(3,offset+9);
  hexahedron_8->GetPointIds()->SetId(4,13);
  hexahedron_8->GetPointIds()->SetId(5,14);
  hexahedron_8->GetPointIds()->SetId(6,offset+14);
  hexahedron_8->GetPointIds()->SetId(7,offset+13);

  vtkSmartPointer<vtkHexahedron> hexahedron_9 = vtkSmartPointer<vtkHexahedron>::New();
  hexahedron_9->GetPointIds()->SetId(0,10);
  hexahedron_9->GetPointIds()->SetId(1,11);
  hexahedron_9->GetPointIds()->SetId(2,offset+11);
  hexahedron_9->GetPointIds()->SetId(3,offset+10);
  hexahedron_9->GetPointIds()->SetId(4,14);
  hexahedron_9->GetPointIds()->SetId(5,15);
  hexahedron_9->GetPointIds()->SetId(6,offset+15);
  hexahedron_9->GetPointIds()->SetId(7,offset+14);

  // Store the cells into a cell array
  vtkSmartPointer<vtkCellArray> cell_array = vtkSmartPointer<vtkCellArray>::New();
  cell_array->InsertNextCell(hexahedron_1);
  cell_array->InsertNextCell(hexahedron_2);
  cell_array->InsertNextCell(hexahedron_3);
  cell_array->InsertNextCell(hexahedron_4);
  cell_array->InsertNextCell(hexahedron_5);
  cell_array->InsertNextCell(hexahedron_6);
  cell_array->InsertNextCell(hexahedron_7);
  cell_array->InsertNextCell(hexahedron_8);
  cell_array->InsertNextCell(hexahedron_9);

  // Set the cell values into an array
  vtkSmartPointer<vtkFloatArray> values_dataset_1 = vtkSmartPointer<vtkFloatArray>::New();
  vtkSmartPointer<vtkFloatArray> values_dataset_2 = vtkSmartPointer<vtkFloatArray>::New();
  for (int i = 0; i < 9; i++)
  {
    values_dataset_1->InsertNextValue(f1(i));
    values_dataset_2->InsertNextValue(f2(i-3));
  }

  // Build the vtkUnstructuredGrid
  vtkSmartPointer<vtkUnstructuredGrid> unstructured_grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
  unstructured_grid->SetPoints(points);
  unstructured_grid->SetCells(VTK_HEXAHEDRON,cell_array);
  unstructured_grid->GetCellData()->SetScalars(values_dataset_1);

  // Write the vtkUnstructuredGrid into a file
  vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
  writer->SetFileName("outputs/dataset_1.vtu");
  writer->SetInputData(unstructured_grid);
  writer->Write();

  unstructured_grid->GetCellData()->SetScalars(values_dataset_2);
  writer->SetFileName("outputs/dataset_2.vtu");
  writer->SetInputData(unstructured_grid);
  writer->Write();

  // ------------------------------------------------------------------------------------------------------------------------------
  // READER
  vtkSmartPointer<vtkXMLUnstructuredGridReader> reader = vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
  reader->SetFileName("outputs/dataset_1.vtu");
  reader->Update();

  vtkUnstructuredGrid *unstructured_grid_2 = reader->GetOutput();

  // Read cells
  int num_cells = unstructured_grid_2->GetNumberOfCells();
  int num_points = unstructured_grid_2->GetNumberOfPoints();

  cout << "Number of points = " << num_points << endl;
  cout << "Number of cells = " << num_cells << endl;

  for (int i = 0; i < num_cells; i++)
  {
    cout << "Cell " << i << endl;

    vtkCell *cell = unstructured_grid->GetCell(i);

    vtkHexahedron *hexahedron = dynamic_cast<vtkHexahedron*>(cell);

    // Read points from each cell
    for (int j = 0; j < 8; j++)
    {
      double pos[3];

      hexahedron->GetPoints()->GetPoint(j,pos);

      cout << "\tPoint " << j << " = (" << pos[0] << "," << pos[1] << "," << pos[2] << ")" << endl;
    }
  }

  // Read cells scalar values
  string array_name = "Scalars_";

  vtkSmartPointer<vtkFloatArray> array = vtkFloatArray::SafeDownCast(unstructured_grid_2->GetCellData()->GetArray(array_name.c_str()));

  if(array)
  {
      // Pass through each cell extracting the scalar value
      for(int i = 0; i < num_cells; i++)
      {
          double value = array->GetValue(i);

          cout << "Cell " << i << " -- Scalar value = " << value << endl;
      }
  }

  return 0;

}

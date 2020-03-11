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

#define SIDE_LENGTH_X 15
#define SIDE_LENGTH_Y 15
#define DX 5
#define DY 5
#define DZ 5


int main (int argc, char *argv[])
{
  // -------------------------------------------------------------------------------------------------------------------------------
  // WRITER
  // Create all the points
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  
  uint32_t counter = 0;
	uint32_t nx = nearbyint(SIDE_LENGTH_X/DX);
	uint32_t ny = nearbyint(SIDE_LENGTH_Y/DY);

	for (uint32_t k = 0; k < 2; k++)
	{
		double z = k*DZ;
		for (uint32_t i = 0; i <= ny; i++)
		{
			double y = i*DY;
			for (uint32_t j = 0; j <= nx; j++)
			{
				double x = j*DX;

				//printf("Point %u = (%g,%g,%g)\n",counter,x,y,z);

        points->InsertNextPoint(x,y,z);

				counter++;
			}
		}	
	}

  // Create all the cells
  vtkSmartPointer<vtkCellArray> cell_array = vtkSmartPointer<vtkCellArray>::New();
  counter = 0;

	for (uint32_t i = 0; i < ny; i++)
	{
		for (uint32_t j = 0; j < nx; j++)
		{
			uint32_t k = i*(ny+1) + j;

      vtkSmartPointer<vtkHexahedron> hexahedron = vtkSmartPointer<vtkHexahedron>::New();

      hexahedron->GetPointIds()->SetId(0,k);
      hexahedron->GetPointIds()->SetId(1,k+1);
      hexahedron->GetPointIds()->SetId(2,k+(nx+1)+1);
      hexahedron->GetPointIds()->SetId(3,k+(nx+1));
      hexahedron->GetPointIds()->SetId(4,k+((nx+1)*(ny+1)));
      hexahedron->GetPointIds()->SetId(5,k+((nx+1)*(ny+1))+1);
      hexahedron->GetPointIds()->SetId(6,k+((nx+1)*(ny+1))+(nx+1)+1);
      hexahedron->GetPointIds()->SetId(7,k+((nx+1)*(ny+1))+(nx+1));

      cell_array->InsertNextCell(hexahedron);

			//printf("Cell %u = [%u %u %u %u %u %u %u %u]\n",counter,\
															k, k+1,\
															k+(nx+1), k+(nx+1)+1,\
															k+((nx+1)*(ny+1)),k+((nx+1)*(ny+1))+1,\
															k+((nx+1)*(ny+1))+(nx+1),k+((nx+1)*(ny+1))+(nx+1)+1);

			counter++;
		}
	}

  // Set the cell values into an array
  //vtkSmartPointer<vtkFloatArray> values_dataset_1 = vtkSmartPointer<vtkFloatArray>::New();
  //vtkSmartPointer<vtkFloatArray> values_dataset_2 = vtkSmartPointer<vtkFloatArray>::New();
  //for (int i = 0; i < 9; i++)
  //{
  //  values_dataset_1->InsertNextValue(f1(i));
  //  values_dataset_2->InsertNextValue(f2(i-3));
  //}

  // Build the vtkUnstructuredGrid
  vtkSmartPointer<vtkUnstructuredGrid> unstructured_grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
  unstructured_grid->SetPoints(points);
  unstructured_grid->SetCells(VTK_HEXAHEDRON,cell_array);
  //unstructured_grid->GetCellData()->SetScalars(values_dataset_1);

  // Write the vtkUnstructuredGrid into a file
  vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
  writer->SetFileName("outputs/output.vtu");
  writer->SetInputData(unstructured_grid);
  writer->Write();

  return 0;

}

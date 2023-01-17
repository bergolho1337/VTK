// Program to test line-line collision detection using the VTK library

#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkSmartPointer.h>
#include <vtkNamedColors.h>
#include <vtkLine.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkXMLPolyDataWriter.h>

using namespace std;

int main(int, char *argv[])
{
  double p1[3], p2[3], p3[3], p4[3];
  scanf("%lf %lf %lf",&p1[0],&p1[1],&p1[2]);
  scanf("%lf %lf %lf",&p2[0],&p2[1],&p2[2]);
  scanf("%lf %lf %lf",&p3[0],&p3[1],&p3[2]);
  scanf("%lf %lf %lf",&p4[0],&p4[1],&p4[2]);

  // Build lines
  vtkSmartPointer<vtkPolyData> polydata_1 = vtkSmartPointer<vtkPolyData>::New();
  vtkSmartPointer<vtkPolyData> polydata_2 = vtkSmartPointer<vtkPolyData>::New();

  vtkSmartPointer<vtkPoints> points_1 = vtkSmartPointer<vtkPoints>::New();
  points_1->InsertNextPoint(p1);
  points_1->InsertNextPoint(p2);

  vtkSmartPointer<vtkPoints> points_2 = vtkSmartPointer<vtkPoints>::New();
  points_2->InsertNextPoint(p3);
  points_2->InsertNextPoint(p4);

  polydata_1->SetPoints(points_1);
  polydata_2->SetPoints(points_2);

  vtkSmartPointer<vtkLine> line_1 = vtkSmartPointer<vtkLine>::New();
  line_1->GetPointIds()->SetId(0,0);
  line_1->GetPointIds()->SetId(1,1);

  vtkSmartPointer<vtkLine> line_2 = vtkSmartPointer<vtkLine>::New();
  line_2->GetPointIds()->SetId(0,0);
  line_2->GetPointIds()->SetId(1,1);

  vtkSmartPointer<vtkCellArray> cell_array_1 = vtkSmartPointer<vtkCellArray>::New();
  cell_array_1->InsertNextCell(line_1);

  vtkSmartPointer<vtkCellArray> cell_array_2 = vtkSmartPointer<vtkCellArray>::New();
  cell_array_2->InsertNextCell(line_2);

  polydata_1->SetLines(cell_array_1);

  polydata_2->SetLines(cell_array_2);

  vtkSmartPointer<vtkXMLPolyDataWriter> writer_1 = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer_1->SetFileName("outputs/line_1.vtp");
  writer_1->SetInputData(polydata_1);
  writer_1->Write();

  vtkSmartPointer<vtkXMLPolyDataWriter> writer_2 = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer_2->SetFileName("outputs/line_2.vtp");
  writer_2->SetInputData(polydata_2);
  writer_2->Write();

  // Check intersection
  double t;
  double x[3];
  double pcoords[3];
  int subId;

  line_1->GetPoints()->SetPoint(0,p1);
  line_1->GetPoints()->SetPoint(1,p2);
  line_2->GetPoints()->SetPoint(0,p3);
  line_2->GetPoints()->SetPoint(1,p4);
  int ret = line_1->IntersectWithLine(p3, p4, 1e+06, t, x, pcoords, subId);
  double dist = sqrt(pow(x1[0]-x2[0],2)+pow(x1[1]-x2[1],2)+pow(x1[2]-x2[2],2));

  std::cout << "ret: " << ret_1 << std::endl; // Returns 1 if lines intersect
  std::cout << "t: " << t << std::endl; // Return parametric position of the intersection [0,1]
  std::cout << "subId: " << subId << std::endl; // Index
  std::cout << "x: " << x[0] << " " << x[1] << " " << x[2] << std::endl; // Intersection coordinates
  std::cout << "pcoords: " << pcoords[0] << " " << pcoords[1] << " " << pcoords[2] << std::endl;
  std::cout << "dist:" << dist << std::endl;
}

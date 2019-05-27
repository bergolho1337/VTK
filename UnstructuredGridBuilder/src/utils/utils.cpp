#include "utils.h"

void usage (const char pname[])
{
    printf("%s\n",PRINT_LINE);
    printf("Usage:> %s <input_file>\n",pname);
    printf("%s\n",PRINT_LINE);
    printf("<input_file> = File with (x,y,z) positions from the points\n");
    printf("%s\n",PRINT_LINE);
    printf("Example:\n");
    printf("%s <input_file>\n",pname);
    printf("%s\n",PRINT_LINE);
}


std::ostream& operator<<(std::ostream& os, const Point_3D& obj)
{
    os << "(" << obj.x << "," << obj.y << "," << obj.z << ")";
    return os;
}

void read_points_from_file (std::vector<Point_3D> &points, const std::string filename)
{
    FILE *file = fopen(filename.c_str(),"r");

    double p[3];
    while (fscanf(file,"%lf %lf %lf",&p[0],&p[1],&p[2]) != EOF)
    {
        Point_3D point(p[0],p[1],p[2]);

        points.push_back(point);
    }

    fclose(file);
}

void write_vtu_from_points (std::vector<Point_3D> centers)
{
    double p1[3];
    double p2[3];
    double p3[3];
    double p4[3];
    double p5[3];
    double p6[3];
    double p7[3];
    double p8[3];

    double half_face_x = DX / 2.0;
    double half_face_y = DY / 2.0;
    double half_face_z = DZ / 2.0;

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    uint32_t current_number_points = 0;

    vtkSmartPointer<vtkCellArray> cell_array = vtkSmartPointer<vtkCellArray>::New();
    uint32_t current_number_cells = 0;

    for (uint32_t i = 0; i < centers.size(); i++)
    {
        double center_x = centers[i].x;
        double center_y = centers[i].y;
        double center_z = centers[i].z;

        p1[0] = center_x - half_face_x;
        p1[1] = center_y - half_face_y;
        p1[2] = center_z - half_face_z;

        p2[0] = center_x + half_face_x;
        p2[1] = center_y - half_face_y;
        p2[2] = center_z - half_face_z;

        p3[0] = center_x + half_face_x;
        p3[1] = center_y + half_face_y;
        p3[2] = center_z - half_face_z;

        p4[0] = center_x - half_face_x;
        p4[1] = center_y + half_face_y;
        p4[2] = center_z - half_face_z;

        p5[0] = center_x - half_face_x;
        p5[1] = center_y - half_face_y;
        p5[2] = center_z + half_face_z;

        p6[0] = center_x + half_face_x;
        p6[1] = center_y - half_face_y;
        p6[2] = center_z + half_face_z;

        p7[0] = center_x + half_face_x;
        p7[1] = center_y + half_face_y;
        p7[2] = center_z + half_face_z;

        p8[0] = center_x - half_face_x;
        p8[1] = center_y + half_face_y;
        p8[2] = center_z + half_face_z;

        points->InsertNextPoint(p1[0],p1[1],p1[2]);
        points->InsertNextPoint(p2[0],p2[1],p2[2]);
        points->InsertNextPoint(p3[0],p3[1],p3[2]);
        points->InsertNextPoint(p4[0],p4[1],p4[2]);
        points->InsertNextPoint(p5[0],p5[1],p5[2]);
        points->InsertNextPoint(p6[0],p6[1],p6[2]);
        points->InsertNextPoint(p7[0],p7[1],p7[2]);
        points->InsertNextPoint(p8[0],p8[1],p8[2]);


        vtkSmartPointer<vtkHexahedron> hexahedron = vtkSmartPointer<vtkHexahedron>::New();
        hexahedron->GetPointIds()->SetId(0, current_number_points);
        hexahedron->GetPointIds()->SetId(1, current_number_points+1);
        hexahedron->GetPointIds()->SetId(2, current_number_points+2);
        hexahedron->GetPointIds()->SetId(3, current_number_points+3);
        hexahedron->GetPointIds()->SetId(4, current_number_points+4);
        hexahedron->GetPointIds()->SetId(5, current_number_points+5);
        hexahedron->GetPointIds()->SetId(6, current_number_points+6);
        hexahedron->GetPointIds()->SetId(7, current_number_points+7);

        cell_array->InsertNextCell(hexahedron);

        current_number_points += 8;
        current_number_cells++;
        
    }
  
    vtkSmartPointer<vtkUnstructuredGrid> unstructured_grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    unstructured_grid->SetPoints(points);
    unstructured_grid->SetCells(VTK_HEXAHEDRON, cell_array);

    vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
    writer->SetFileName("outputs/output.vtu");
    writer->SetInputData(unstructured_grid);
    writer->Write();

}
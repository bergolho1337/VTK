#include "control_volume.h"

void read_data_inside_bounds_from_vtu(std::string input_file,\
                                const double min_x, const double max_x,\
                                const double min_y, const double max_y,\
                                const double min_z, const double max_z)
{
    // Array to store all the points inside the bounds
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    // Map to store the numbering of the points
    std::map<Point_3D,uint32_t> points_map;
    std::map<Point_3D,uint32_t>::iterator it;

    // Read the input VTU file
    vtkSmartPointer<vtkXMLUnstructuredGridReader> reader = vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
    reader->SetFileName(input_file.c_str());
    reader->Update();

    vtkUnstructuredGrid *unstructured_grid = reader->GetOutput();

    uint32_t total_num_cells = unstructured_grid->GetNumberOfCells();
    uint32_t total_num_points = unstructured_grid->GetNumberOfPoints();

    uint32_t points_inside_bounds = 0;

    for (uint32_t i = 0; i < total_num_cells; i++)
    {
        vtkCell *cell = unstructured_grid->GetCell(i);

        vtkHexahedron *hexahedron = dynamic_cast<vtkHexahedron*>(cell);

        double center[3];
        calc_control_volume_center_position(hexahedron,center);
        //printf("(%g,%g,%g)\n",center[0],center[1],center[2]);

        // Check if the center of the current cell is inside the bounds
        if (is_inside_bounds(center,min_x,max_x,min_y,max_y,min_z,max_z))
        {
            double p[3];
            // Pass through all the points of that cell and try to insert it on the map and array
            for (uint32_t j = 0; j < 8; j++)
            {
                hexahedron->GetPoints()->GetPoint(j, p);

                Point_3D point(p[0],p[1],p[2]);

                if (points_map.find(point) == points_map.end())
                {
                    points_map.insert(std::pair<Point_3D,uint32_t>(point,points_inside_bounds));
                    
                    points->InsertNextPoint(p[0],p[1],p[2]);
                    //printf("(%g,%g,%g)\n",p[0],p[1],p[2]);
                    
                    points_inside_bounds++;
                }
            }
        }
    }

    vtkSmartPointer<vtkCellArray> cell_array = vtkSmartPointer<vtkCellArray>::New();

    for (uint32_t i = 0; i < total_num_cells; i++)
    {
        vtkCell *cell = unstructured_grid->GetCell(i);

        vtkHexahedron *hexahedron = dynamic_cast<vtkHexahedron*>(cell);

        double center[3];
        calc_control_volume_center_position(hexahedron,center);
        //printf("(%g,%g,%g)\n",center[0],center[1],center[2]);

        // Check if the center of the current cell is inside the bounds
        if (is_inside_bounds(center,min_x,max_x,min_y,max_y,min_z,max_z))
        {
            uint32_t id0;
            uint32_t id1;
            uint32_t id2;
            uint32_t id3;
            uint32_t id4;
            uint32_t id5;
            uint32_t id6;
            uint32_t id7;

            double p0[3];
            double p1[3];
            double p2[3];
            double p3[3];
            double p4[3];
            double p5[3];
            double p6[3];
            double p7[3];

            hexahedron->GetPoints()->GetPoint(0, p0);
            hexahedron->GetPoints()->GetPoint(1, p1);
            hexahedron->GetPoints()->GetPoint(2, p2);
            hexahedron->GetPoints()->GetPoint(3, p3);
            hexahedron->GetPoints()->GetPoint(4, p4);
            hexahedron->GetPoints()->GetPoint(5, p5);
            hexahedron->GetPoints()->GetPoint(6, p6);
            hexahedron->GetPoints()->GetPoint(7, p7);

            Point_3D point_0(p0[0],p0[1],p0[2]);
            it = points_map.find(point_0); 
            if (it != points_map.end())
                id0 = it->second;
            
            Point_3D point_1(p1[0],p1[1],p1[2]);
            it = points_map.find(point_1); 
            if (it != points_map.end())
                id1 = it->second;

            Point_3D point_2(p2[0],p2[1],p2[2]);
            it = points_map.find(point_2); 
            if (it != points_map.end())
                id2 = it->second;

            Point_3D point_3(p3[0],p3[1],p3[2]);
            it = points_map.find(point_3); 
            if (it != points_map.end())
                id3 = it->second;

            Point_3D point_4(p4[0],p4[1],p4[2]);
            it = points_map.find(point_4); 
            if (it != points_map.end())
                id4 = it->second;

            Point_3D point_5(p5[0],p5[1],p5[2]);
            it = points_map.find(point_5); 
            if (it != points_map.end())
                id5 = it->second;

            Point_3D point_6(p6[0],p6[1],p6[2]);
            it = points_map.find(point_6); 
            if (it != points_map.end())
                id6 = it->second;

            Point_3D point_7(p7[0],p7[1],p7[2]);
            it = points_map.find(point_7); 
            if (it != points_map.end())
                id7 = it->second;

            vtkSmartPointer<vtkHexahedron> hexahedron = vtkSmartPointer<vtkHexahedron>::New();
            hexahedron->GetPointIds()->SetId(0, id0);
            hexahedron->GetPointIds()->SetId(1, id1);
            hexahedron->GetPointIds()->SetId(2, id2);
            hexahedron->GetPointIds()->SetId(3, id3);
            hexahedron->GetPointIds()->SetId(4, id4);
            hexahedron->GetPointIds()->SetId(5, id5);
            hexahedron->GetPointIds()->SetId(6, id6);
            hexahedron->GetPointIds()->SetId(7, id7);

            cell_array->InsertNextCell(hexahedron);
        }
    }

    vtkSmartPointer<vtkUnstructuredGrid> unstructured_grid_new = vtkSmartPointer<vtkUnstructuredGrid>::New();
    unstructured_grid_new->SetPoints(points);
    unstructured_grid_new->SetCells(VTK_HEXAHEDRON, cell_array);

    vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
    writer->SetFileName("outputs/output.vtu");
    writer->SetInputData(unstructured_grid_new);
    writer->Write();
}

void calc_control_volume_center_position(vtkHexahedron *hexahedron, double center[])
{
    double p0[3];
    double p1[3];
    double p2[3];
    double p3[3];
    double p4[3];
    double p5[3];
    double p6[3];
    double p7[3];

    hexahedron->GetPoints()->GetPoint(0, p0);
    hexahedron->GetPoints()->GetPoint(1, p1);
    hexahedron->GetPoints()->GetPoint(2, p2);
    hexahedron->GetPoints()->GetPoint(3, p3);
    hexahedron->GetPoints()->GetPoint(4, p4);
    hexahedron->GetPoints()->GetPoint(5, p5);
    hexahedron->GetPoints()->GetPoint(6, p6);
    hexahedron->GetPoints()->GetPoint(7, p7);

    center[0] = (p0[0] + p1[0]) / 2.0;
    center[1] = (p0[1] + p3[1]) / 2.0;
    center[2] = (p0[2] + p4[2]) / 2.0;
}

bool is_inside_bounds(const double center[],\
                    const double min_x, const double max_x,\
                    const double min_y, const double max_y,\
                    const double min_z, const double max_z)
{
    bool ignore = center[0] < min_x || center[0] > max_x || center[1] < min_y || center[1] > max_y ||
                                   center[2] < min_z || center[2] > max_z;
    
    return !ignore;
}
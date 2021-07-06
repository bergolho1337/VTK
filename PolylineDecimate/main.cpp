#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkLine.h>
#include <vtkPolyLine.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkDecimatePolylineFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkXMLPolyDataWriter.h>

int main ()
{
    const uint32_t NUM_POINTS = 100;

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    for (uint32_t i = 0; i < NUM_POINTS; i++)
    {
        double p[3] = {(double)i,0.0,0.0};
        points->InsertNextPoint(p);
    }

    vtkSmartPointer<vtkPolyLine> polyline = vtkSmartPointer<vtkPolyLine>::New();
    polyline->GetPointIds()->SetNumberOfIds(NUM_POINTS);
    for (uint32_t i = 0; i < NUM_POINTS; i++)
        polyline->GetPointIds()->SetId(i, i);

    vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
    cells->InsertNextCell(polyline);

    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(points);
    polydata->SetLines(cells);

    vtkSmartPointer<vtkDecimatePolylineFilter> decimate = vtkSmartPointer<vtkDecimatePolylineFilter>::New();
    decimate->SetInputData(polydata);
    decimate->SetTargetReduction(0.95);
    decimate->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(decimate->GetOutputPort());

    vtkSmartPointer<vtkPolyData> decimate_polydata = mapper->GetInput();

    //vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    //writer->SetFileName("outputs/polyline.vtp");
    //writer->SetFileName("outputs/polyline_decimate.vtp");
    //writer->SetInputData(polydata);
    //writer->SetInputData(decimate_polydata);
    //writer->Write();

    return 0;
}
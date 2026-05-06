#include <vtkXMLUnstructuredGridReader.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkSubdivideTetra.h>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " input.vtu output_refined.vtu\n";
        return EXIT_FAILURE;
    }

    auto reader = vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
    reader->SetFileName(argv[1]);
    reader->Update();

    auto subdivide = vtkSmartPointer<vtkSubdivideTetra>::New();
    subdivide->SetInputConnection(reader->GetOutputPort());
    subdivide->Update();

    auto writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
    writer->SetFileName(argv[2]);
    writer->SetInputConnection(subdivide->GetOutputPort());
    writer->Write();

    return EXIT_SUCCESS;
}

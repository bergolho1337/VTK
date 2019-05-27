// Author: Lucas Berg
// ---------------------------------------------------------------------------------------------
// Program that reads the positions from a set points and write a VTK_HEXAEDRON on that place.
// ---------------------------------------------------------------------------------------------

#include <iostream>
#include <vector>

#include "utils/utils.h"

using namespace std;

int main (int argc, char *argv[])
{
    if (argc-1 != 1)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    string input_file = argv[1];

    vector<Point_3D> points;
    read_points_from_file(points,input_file);

    write_vtu_from_points(points);

    return 0;
}
// Author: Lucas Berg
// ---------------------------------------------------------------------------------------------
// Program that reads VTU file and generate a second one that only contains the cells inside
// a boundary which is given as an input.
// ---------------------------------------------------------------------------------------------

#include <iostream>
#include <vector>

#include "utils/utils.h"
#include "tissue/control_volume.h"

using namespace std;

int main (int argc, char *argv[])
{
    if (argc-1 != 7)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    string input_file = argv[1];
    double min_x = atof(argv[2]);
    double max_x = atof(argv[3]);
    double min_y = atof(argv[4]);
    double max_y = atof(argv[5]);
    double min_z = atof(argv[6]);
    double max_z = atof(argv[7]);

    read_data_inside_bounds_from_vtu(input_file,min_x,max_x,\
                                min_y,max_y,\
                                min_z,max_z);

    return 0;
}
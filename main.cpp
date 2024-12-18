/* Glushkova Eva st108255@student.spbu.ru
Lab1 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "bmp_struct.h"
#include "bmp_file.h"

int main()
{
    const char my_file[] = "my_file.bmp"; // change the paths
    const char left90Clock[] = "left90Clock.bmp";
    const char right90Clock[] = "right90Clock.bmp";
    const char gauss[] = "gaussian.bmp";


    BMPfile bmp = loadfile(my_file);

    if (LeftClock90(bmp, left90Clock))
    {
        std::cout << "Left90Clock is done!" << "\n";
    }

    else
    {
        std::cerr << "Error LeftClock90" << "\n";
    }

    if (RightClock90(bmp, right90Clock))
    {
        std::cout << "RightClock90 is done!" << "\n";
    }

    else
    {
        std::cerr << "Error RightClock90" << "\n";
    }

    if (Gauss(bmp, gauss))
    {
        std::cout << "Gauss is done!" << "\n";
    }

    else
    {
        std::cerr << "Error Gauss" << "\n";
    }



}

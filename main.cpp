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


    if (Clock90_and_Gaussian(my_file, left90Clock, right90Clock, gauss))
    {
        std::cout << "Left90Clock, Right90Clock and Gauss is done!" << "\n";
    }
    else
    {
        std::cerr << "Error Clock90_and_Gaussian" << "\n";
    }



}

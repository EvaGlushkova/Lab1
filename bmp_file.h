/* Glushkova Eva st108255@student.spbu.ru
Lab1 */

#include "bmp_struct.h"
#include <vector>
#include <string>
#include <iostream>

#ifndef BMP_FILE_H
#define BMP_FILE_H

bool Clock90_and_Gaussian(const char *file_name, const char *left, const char *right, const char *gauss);
std::vector<unsigned char> pic_Gauss(std::vector<unsigned char>& file, int height, int width);


#endif

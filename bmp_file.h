/* Glushkova Eva st108255@student.spbu.ru
Lab1 */

#include "bmp_struct.h"
#include <vector>
#include <string>
#include <iostream>

#ifndef BMP_FILE_H
#define BMP_FILE_H

BMPfile loadfile(const char *file_name);
bool LeftClock90(BMPfile& original, const char *left);
bool RightClock90(BMPfile& original, const char *right);
bool Gauss(BMPfile& original, const char *gauss);
std::vector<unsigned char> pic_Gauss(std::vector<unsigned char>& file, int height, int width);


#endif

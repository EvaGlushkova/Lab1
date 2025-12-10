/* Glushkova Eva st108255@student.spbu.ru
Lab1 */

#include "bmp_struct.h"
#include <vector>
#include <string>
#include <iostream>

#ifndef BMP_FILE_H
#define BMP_FILE_H
/**
 * @file bmp_file.h
 * @brief Header for BMP image processing functions
 */

/**
 * @brief Loads BMP file from disk
 * @param file_name Path to BMP file
 * @return BMPfile structure
 */
BMPfile loadfile(const char *file_name);
/**
 * @brief Rotates image 90 degrees left
 * @return True if successful
 */
bool LeftClock90(BMPfile& original, const char *left);
/**
 * @brief Rotates image 90 degrees right
 * @return True if successful
 */
bool RightClock90(BMPfile& original, const char *right);
/**
 * @brief Applies Gaussian filter
 * @return True if successful
 */
bool Gauss(BMPfile& original, const char *gauss);
/**
 * @brief Internal Gaussian filter function
 * @return Filtered pixel data
 */
std::vector<unsigned char> pic_Gauss(std::vector<unsigned char>& file, int height, int width);

#endif
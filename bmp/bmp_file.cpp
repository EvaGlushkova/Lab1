/* Glushkova Eva st108255@student.spbu.ru
Lab1 */
#include "bmp_file.h"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <cmath>

/**
 * @file bmp_file.cpp
 * @brief Implementation of BMP image processing functions
 */

/**
 * @brief Loads a BMP file from disk
 * @param file_name Path to the BMP file
 * @return BMPfile structure with image data
 */
BMPfile loadfile(const char *file_name)
{
    std::ifstream original_file(file_name, std::ios::binary);
    if (!original_file)
    {
        std::cout << "Error opening the first file" << "\n";
        exit(1);
    }

    BMPfile bmp;

    original_file.read(reinterpret_cast<char*>(&bmp.BitMapFileHeader), sizeof(BITMAPFILEHEADER));
    original_file.read(reinterpret_cast<char*>(&bmp.BitMapInfo), sizeof(BITMAPINFO));

    if (bmp.BitMapInfo.bitcount != 8)
    {
        std::cout<<bmp.BitMapInfo.bitcount<< " bits" <<  "\n";
        std::cerr << "NOT 8 bit" << "\n";
        original_file.close();
        exit(1);
    }

    bmp.pix.resize(bmp.BitMapInfo.colors);


    original_file.seekg(bmp.BitMapFileHeader.bfOffBits - sizeof(RGB) * bmp.BitMapInfo.colors);
    original_file.read(reinterpret_cast<char*>(bmp.pix.data()), sizeof(RGB) * bmp.BitMapInfo.colors);
    original_file.seekg(bmp.BitMapFileHeader.bfOffBits);


    uint32_t width = bmp.BitMapInfo.width;
    uint32_t height = bmp.BitMapInfo.height;

    bmp.pic_data.resize(height * width);

    original_file.read(reinterpret_cast<char*>(bmp.pic_data.data()), height * width);

    original_file.close();

    return bmp;
}
/**
 * @brief Rotates image 90 degrees counter-clockwise
 * @return True if successful
 */
bool LeftClock90(BMPfile& original, const char *left)
{
    std::ofstream left_clock(left, std::ios::binary);

    if (!left_clock)
    {
        std::cerr << "Error opening the left_clock" << std::endl;
        return false;
    }

    BITMAPFILEHEADER BitMapFileHeader = original.BitMapFileHeader;
    BITMAPINFO BitMapInfo = original.BitMapInfo;

    uint32_t width = original.BitMapInfo.width;
    uint32_t height = original.BitMapInfo.height;

    BitMapInfo.width = height;
    BitMapInfo.height = width;

    left_clock.write(reinterpret_cast<char*>(&BitMapFileHeader), sizeof(BITMAPFILEHEADER));
    left_clock.write(reinterpret_cast<char*>(&BitMapInfo), sizeof(BITMAPINFO));


    left_clock.seekp(BitMapFileHeader.bfOffBits - sizeof(RGB) * BitMapInfo.colors);
    left_clock.write(reinterpret_cast<char*>(original.pix.data()), sizeof(RGB) * original.pix.size());
    left_clock.seekp(BitMapFileHeader.bfOffBits);

    std::vector<unsigned char> left90clock(height);

    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {

            left90clock[y] = original.pic_data[y * width + x];

        }
        for (int y = 0; y < height; ++y)
        {

            left_clock.put(left90clock[height - y - 1]);
        }

    }


    left_clock.close();

    return true;

}
/**
 * @brief Rotates image 90 degrees clockwise
 * @return True if successful
 */

bool RightClock90(BMPfile& original, const char *right)
{
    std::ofstream right_clock(right, std::ios::binary);

    if (!right_clock)
    {
        std::cerr << "Error opening the RightClock90 file" << std::endl;
        return false;
    }

    BITMAPFILEHEADER BitMapFileHeader = original.BitMapFileHeader;
    BITMAPINFO BitMapInfo = original.BitMapInfo;

    uint32_t width = original.BitMapInfo.width;
    uint32_t height = original.BitMapInfo.height;

    BitMapInfo.width = height;
    BitMapInfo.height = width;

    right_clock.write(reinterpret_cast<char*>(&BitMapFileHeader), sizeof(BITMAPFILEHEADER));
    right_clock.write(reinterpret_cast<char*>(&BitMapInfo), sizeof(BITMAPINFO));

    right_clock.seekp(BitMapFileHeader.bfOffBits - sizeof(RGB) * BitMapInfo.colors);
    right_clock.write(reinterpret_cast<char*>(original.pix.data()), sizeof(RGB) * original.pix.size());
    right_clock.seekp(BitMapFileHeader.bfOffBits);

    for (int x = 0; x < width; ++x)
    {
        for (int y = height - 1; y >= 0; --y)
        {
            unsigned char pixel = original.pic_data[y * width + x];
            right_clock.put(pixel);
        }
    }

    right_clock.close();
    return true;
}
/**
 * @brief Applies Gaussian filter to image
 * @return True if successful
 */
bool Gauss(BMPfile& original, const char *gauss)
{
    std::ofstream gaussian(gauss, std::ios::binary);

    if (!gaussian)
    {
        std::cerr << "Error opening the gauss file" << std::endl;
        return false;
    }


    gaussian.write(reinterpret_cast<char*>(&original.BitMapFileHeader), sizeof(BITMAPFILEHEADER));
    gaussian.write(reinterpret_cast<char*>(&original.BitMapInfo), sizeof(BITMAPINFO));

    gaussian.seekp(original.BitMapFileHeader.bfOffBits - sizeof(RGB) * original.BitMapInfo.colors);
    gaussian.write(reinterpret_cast<char*>(original.pix.data()), sizeof(RGB) * original.pix.size());
    gaussian.seekp(original.BitMapFileHeader.bfOffBits);

    std::vector<unsigned char> pic_gauss = pic_Gauss(original.pic_data, original.BitMapInfo.height, original.BitMapInfo.width);

    gaussian.write(reinterpret_cast<char*>(pic_gauss.data()), original.BitMapInfo.height * original.BitMapInfo.width);

    gaussian.close();

    return true;

}

/**
 * @brief Internal Gaussian filter implementation
 * @return Filtered pixel data
 */
std::vector<unsigned char> pic_Gauss(std::vector<unsigned char>& file, int height, int width)
{
    double gauss_kernel[5][5] = {
        {5, 8, 10, 8, 5},
        {8, 20, 80, 20, 8},
        {10, 80, 120, 80, 10},
        {8, 20, 80, 20, 8},
        {5, 8, 10, 8, 5}
    };

    double sum_kernel = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            sum_kernel += gauss_kernel[i][j];
        }
    }

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            gauss_kernel[i][j] = gauss_kernel[i][j] / sum_kernel;
        }
    }

    std::vector<unsigned char> pic_gauss(height * width);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            
            if (y < 2  || y >= height - 2 || x < 2 || x >= width - 2) {
                pic_gauss[x + y * width] = file[x + y * width];
                continue;
            }

            double pix = 0;

            for (int ker_y = -2; ker_y <= 2; ++ker_y) {
                for (int ker_x = -2; ker_x <= 2; ++ker_x) {
                    int idx = (y + ker_y) * width + (x + ker_x);
                    pix += file[idx] * gauss_kernel[ker_y + 2][ker_x + 2];
                }
            }

            if (pix < 0) pix = 0;
            if (pix > 255) pix = 255;
            
            pic_gauss[x + y * width] = static_cast<unsigned char>(pix);
        }
    }
    return pic_gauss;
}

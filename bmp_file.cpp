#include "bmp_file.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <cmath>


bool Clock90_and_Gaussian(const char *file_name, const char *left, const char *right, const char *gauss)
{

    std::ifstream original_file(file_name, std::ios::binary);

    if (!original_file)
    {
        std::cerr << "Error opening the first file" << std::endl;
        return false;
    }

    std::ofstream left_clock(left, std::ios::binary);

    if (!left_clock)
    {
        std::cerr << "Error opening the second file" << std::endl;
        return false;
    }

    std::ofstream right_clock(right, std::ios::binary);

    if (!right_clock)
    {
        std::cerr << "Error opening the third file" << std::endl;
        return false;
    }

    std::ofstream gaussian(gauss, std::ios::binary);

    if (!gaussian)
    {
        std::cerr << "Error opening the gauss file" << std::endl;
        return false;
    }

    BITMAPFILEHEADER BitMapFileHeader;
    BITMAPINFO BitMapInfo;

    original_file.read(reinterpret_cast<char*>(&BitMapFileHeader), sizeof(BITMAPFILEHEADER));
    original_file.read(reinterpret_cast<char*>(&BitMapInfo), sizeof(BITMAPINFO));

    gaussian.write(reinterpret_cast<char*>(&BitMapFileHeader), sizeof(BITMAPFILEHEADER));
    gaussian.write(reinterpret_cast<char*>(&BitMapInfo), sizeof(BITMAPINFO));

    uint32_t width = BitMapInfo.width;
    uint32_t height = BitMapInfo.height;

    BitMapInfo.width = height;
    BitMapInfo.height = width;

    left_clock.write(reinterpret_cast<char*>(&BitMapFileHeader), sizeof(BITMAPFILEHEADER));
    left_clock.write(reinterpret_cast<char*>(&BitMapInfo), sizeof(BITMAPINFO));

    right_clock.write(reinterpret_cast<char*>(&BitMapFileHeader), sizeof(BITMAPFILEHEADER));
    right_clock.write(reinterpret_cast<char*>(&BitMapInfo), sizeof(BITMAPINFO));

    BitMapInfo.width = width;
    BitMapInfo.height = height;

    if (BitMapInfo.bitcount != 8)
    {
        std::cout<<BitMapInfo.bitcount<< " bits" <<  "\n";
        std::cerr << "NOT 8 bit" << "\n";

        return false;
    }

    std::vector<RGB> pix(BitMapInfo.colors);

    std::vector<unsigned char> pic_data(height * width);

    original_file.seekg(BitMapFileHeader.bfOffBits - sizeof(RGB) * BitMapInfo.colors);
    original_file.read(reinterpret_cast<char*>(pix.data()), sizeof(RGB) * pix.size());
    original_file.seekg(BitMapFileHeader.bfOffBits);

    left_clock.seekp(BitMapFileHeader.bfOffBits - sizeof(RGB) * BitMapInfo.colors);
    left_clock.write(reinterpret_cast<char*>(pix.data()), sizeof(RGB) * pix.size());
    left_clock.seekp(BitMapFileHeader.bfOffBits);

    right_clock.seekp(BitMapFileHeader.bfOffBits - sizeof(RGB) * BitMapInfo.colors);
    right_clock.write(reinterpret_cast<char*>(pix.data()), sizeof(RGB) * pix.size());
    right_clock.seekp(BitMapFileHeader.bfOffBits);

    gaussian.seekp(BitMapFileHeader.bfOffBits - sizeof(RGB) * BitMapInfo.colors);
    gaussian.write(reinterpret_cast<char*>(pix.data()), sizeof(RGB) * pix.size());
    gaussian.seekp(BitMapFileHeader.bfOffBits);


    original_file.read(reinterpret_cast<char*>(pic_data.data()), height * width);

    std::vector<unsigned char> left90clock(width * height);
    std::vector<unsigned char> right90clock(width * height);

    for(int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {

            left90clock[height * (x + 1) - 1 - y] = pic_data[y * width + x]; //(x, y) - pic_data coordinates
            right90clock[y + height * (width - x - 1) ] = pic_data[y * width + x];

        }
    }


    left_clock.write(reinterpret_cast<char*>(left90clock.data()), height * width);
    right_clock.write(reinterpret_cast<char*>(right90clock.data()), height * width);

    std::vector<unsigned char> pic_gauss = pic_Gauss(pic_data, height, width);

    gaussian.write(reinterpret_cast<char*>(pic_gauss.data()), height * width);

    original_file.close();
    left_clock.close();
    right_clock.close();
    gaussian.close();

    return true;

}


std::vector<unsigned char> pic_Gauss(std::vector<unsigned char>& file, int height, int width)
{
    double gauss_kernel[5][5] =           // gauss_kernel radius: int(5 / 2) = 2
    {
        {5, 8, 10, 8, 5},
        {8, 20, 80, 20, 8},
        {10, 80, 120, 80, 10},
        {8, 20, 80, 20, 8},
        {5, 8, 10, 8, 5}
    };

    double sum_kernel = 0;



    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            sum_kernel += gauss_kernel[i][j];
        }
    }

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            gauss_kernel[i][j] =  gauss_kernel[i][j] / sum_kernel;    // sum of gauss_kernel elements is 1
        }
    }

    std::vector<unsigned char> pic_gauss(height * width);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {

            double pix = 0;

            for (int ker_y = -2; ker_y < 3; ++ker_y)
            {

                for (int ker_x = -2; ker_x < 3; ++ker_x)
                {

                    pix += file[width * (y + ker_y) + ker_x + x] * gauss_kernel[2 + ker_y][2 + ker_x];
                }
            }

            pic_gauss[x + y * width] = static_cast<unsigned char>(pix > 255 ? 255 : (pix < 0 ? 0 : pix));
        }
    }
    return pic_gauss;

}


/* Glushkova Eva st108255@student.spbu.ru
Lab1 */

#include <cstdint>
#include <vector>
#include <iostream>
#include <string>

#ifndef BMP_STRUCT_H
#define BMP_STRUCT_H

#pragma pack(push, 1)

struct BITMAPFILEHEADER
{
    unsigned short bfType;
    uint32_t bfsize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    uint32_t bfOffBits;
};

struct BITMAPINFO
{
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitcount;  //pixel
    uint32_t compression;
    uint32_t imagesize;
    uint32_t xperm;
    uint32_t yperm;
    uint32_t colors;
    uint32_t colorsimp;
};

struct RGB
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    unsigned char reserved;
};

#pragma pack(pop)

class BMPfile
{
public:

    BITMAPFILEHEADER BitMapFileHeader;
    BITMAPINFO BitMapInfo;
    std::vector<RGB> pix;
    std::vector<unsigned char> pic_data;  //pixels

    BMPfile();

    ~BMPfile();


};



#endif










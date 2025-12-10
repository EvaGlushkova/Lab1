/* Glushkova Eva st108255@student.spbu.ru
Lab1 */
#ifndef BMP_STRUCT_H
#define BMP_STRUCT_H

#include <cstdint>
#include <vector>

#pragma pack(push, 1)
/**
 * @file bmp_struct.h
 * @brief BMP file structures and class definition
 */

/**
 * @brief BMP file header structure
 */
struct BITMAPFILEHEADER {
    unsigned short bfType;
    uint32_t bfsize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    uint32_t bfOffBits;
};
/**
 * @brief BMP information header structure
 */
struct BITMAPINFO {
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitcount;
    uint32_t compression;
    uint32_t imagesize;
    uint32_t xperm;
    uint32_t yperm;
    uint32_t colors;
    uint32_t colorsimp;
};
/**
 * @brief Color palette entry structure
 */
struct RGB {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    unsigned char reserved;
};

#pragma pack(pop)
/**
 * @brief Container for BMP image data
 */
class BMPfile {
public:
    BITMAPFILEHEADER BitMapFileHeader;
    BITMAPINFO BitMapInfo;
    std::vector<RGB> pix;
    std::vector<unsigned char> pic_data;

    BMPfile();
    ~BMPfile();
};

#endif
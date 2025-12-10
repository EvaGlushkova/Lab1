/* Glushkova Eva st108255@student.spbu.ru
Lab1 */
#include "bmp_parallel.h"
#include "../bmp/bmp_file.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <thread>

/**
 * @file bmp_parallel.cpp
 * @brief Implementation of BMPParallelService class for parallel image processing
 */

/**
 * @brief Constructor initializing thread count
 * @param numThreads Number of threads to use
 */
BMPParallelService::BMPParallelService(int numThreads) : numThreads(numThreads) {}

/**
 * @brief Processes a block of rows for left rotation
 * @param original Source image data
 * @param result Destination buffer
 * @param startY Starting row index
 * @param endY Ending row index
 * @param width Image width
 * @param height Image height
 */
void BMPParallelService::leftRotateBlock(const BMPfile& original, std::vector<unsigned char>& result,
                                         int startY, int endY, int width, int height) {
    int newWidth = height;

    for (int y = startY; y < endY; ++y) {
        const unsigned char* srcRow = original.pic_data.data() + y * width;
        int dstCol = newWidth - 1 - y;

        for (int x = 0; x < width; ++x) {
            result[x * newWidth + dstCol] = srcRow[x];
        }
    }
}
/**
 * @brief Rotates image left using multiple threads
 * @return True if successful
 */
bool BMPParallelService::LeftClock90(BMPfile& original, const char* outputFile) {
    uint32_t width = original.BitMapInfo.width;
    uint32_t height = original.BitMapInfo.height;

    if (width * height < 100000) {
        return ::LeftClock90(original, outputFile);
    }

    std::ofstream output(outputFile, std::ios::binary);
    if (!output) {
        std::cerr << "Error opening the output file: " << outputFile << std::endl;
        return false;
    }

    BITMAPFILEHEADER newHeader = original.BitMapFileHeader;
    BITMAPINFO newInfo = original.BitMapInfo;
    newInfo.width = height;
    newInfo.height = width;

    output.write(reinterpret_cast<char*>(&newHeader), sizeof(BITMAPFILEHEADER));
    output.write(reinterpret_cast<char*>(&newInfo), sizeof(BITMAPINFO));

    output.seekp(newHeader.bfOffBits - sizeof(RGB) * newInfo.colors);
    output.write(reinterpret_cast<char*>(original.pix.data()), sizeof(RGB) * original.pix.size());
    output.seekp(newHeader.bfOffBits);

    std::vector<unsigned char> rotated(height * width);

    std::vector<std::thread> threads;
    int rowsPerThread = height / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int startY = i * rowsPerThread;
        int endY = (i == numThreads - 1) ? height : startY + rowsPerThread;

        threads.emplace_back([&, startY, endY]() {
            leftRotateBlock(original, rotated, startY, endY, width, height);
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    output.write(reinterpret_cast<char*>(rotated.data()), rotated.size());
    output.close();

    return true;
}
/**
 * @brief Processes a block of rows for right rotation
 * @param original Source image data
 * @param result Destination buffer
 * @param startY Starting row index
 * @param endY Ending row index
 * @param width Image width
 * @param height Image height
 */
void BMPParallelService::rightRotateBlock(const BMPfile& original, std::vector<unsigned char>& result,
                                          int startY, int endY, int width, int height) {
    int newWidth = height;

    for (int y = startY; y < endY; ++y) {
        const unsigned char* srcRow = original.pic_data.data() + y * width;
        
        for (int x = 0; x < width; ++x) {
            int newX = height - 1 - y;
            int newY = x;
            result[newY * newWidth + newX] = srcRow[x];
        }
    }
}
/**
 * @brief Rotates image right using multiple threads
 * @return True if successful
 */
bool BMPParallelService::RightClock90(BMPfile& original, const char* outputFile) {
    uint32_t width = original.BitMapInfo.width;
    uint32_t height = original.BitMapInfo.height;

    if (width * height < 100000) {
        return ::RightClock90(original, outputFile);
    }

    std::ofstream output(outputFile, std::ios::binary);
    if (!output) {
        std::cerr << "Error opening the output file: " << outputFile << std::endl;
        return false;
    }

    BITMAPFILEHEADER newHeader = original.BitMapFileHeader;
    BITMAPINFO newInfo = original.BitMapInfo;
    newInfo.width = height;
    newInfo.height = width;

    output.write(reinterpret_cast<char*>(&newHeader), sizeof(BITMAPFILEHEADER));
    output.write(reinterpret_cast<char*>(&newInfo), sizeof(BITMAPINFO));

    output.seekp(newHeader.bfOffBits - sizeof(RGB) * newInfo.colors);
    output.write(reinterpret_cast<char*>(original.pix.data()), sizeof(RGB) * original.pix.size());
    output.seekp(newHeader.bfOffBits);

    std::vector<unsigned char> rotated(height * width);

    std::vector<std::thread> threads;
    int rowsPerThread = height / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int startY = i * rowsPerThread;
        int endY = (i == numThreads - 1) ? height : startY + rowsPerThread;

        threads.emplace_back([&, startY, endY]() {
            rightRotateBlock(original, rotated, startY, endY, width, height);
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    output.write(reinterpret_cast<char*>(rotated.data()), rotated.size());
    output.close();

    return true;
}
/**
 * @brief Processes a block of rows for Gaussian filter
 * @param input Source pixel data
 * @param output Destination buffer
 * @param startY Starting row index
 * @param endY Ending row index
 * @param width Image width
 * @param height Image height
 */
void BMPParallelService::gaussBlock(const std::vector<unsigned char>& input, std::vector<unsigned char>& output,
                                    int startY, int endY, int width, int height) {
    static constexpr double gauss_kernel[5][5] = {
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

    double normalized_kernel[5][5];
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            normalized_kernel[i][j] = gauss_kernel[i][j] / sum_kernel;
        }
    }

    int actualStartY = std::max(startY, 0);
    int actualEndY = std::min(endY, height);

    for (int y = actualStartY; y < actualEndY; ++y) {
        for (int x = 0; x < width; ++x) {

            if (y < 2 || y >= height - 2 || x < 2 || x >= width - 2) {
                output[y * width + x] = input[y * width + x];
                continue;
            }

            double pix = 0.0;

            for (int ky = -2; ky <= 2; ++ky) {
                for (int kx = -2; kx <= 2; ++kx) {
                    int srcY = y + ky;
                    int srcX = x + kx;
                    int idx = srcY * width + srcX;
                    pix += input[idx] * normalized_kernel[ky + 2][kx + 2];
                }
            }

            if (pix < 0) pix = 0;
            if (pix > 255) pix = 255;

            output[y * width + x] = static_cast<unsigned char>(pix);
        }
    }
}
/**
 * @brief Applies Gaussian filter using multiple threads
 * @return True if successful
 */
bool BMPParallelService::Gauss(BMPfile& original, const char* outputFile) {
    uint32_t width = original.BitMapInfo.width;
    uint32_t height = original.BitMapInfo.height;

    if (width * height < 50000) {
        return ::Gauss(original, outputFile);
    }

    std::ofstream output(outputFile, std::ios::binary);
    if (!output) {
        std::cerr << "Error opening the output file: " << outputFile << std::endl;
        return false;
    }

    output.write(reinterpret_cast<char*>(&original.BitMapFileHeader), sizeof(BITMAPFILEHEADER));
    output.write(reinterpret_cast<char*>(&original.BitMapInfo), sizeof(BITMAPINFO));

    output.seekp(original.BitMapFileHeader.bfOffBits - sizeof(RGB) * original.BitMapInfo.colors);
    output.write(reinterpret_cast<char*>(original.pix.data()), sizeof(RGB) * original.pix.size());
    output.seekp(original.BitMapFileHeader.bfOffBits);

   
    std::vector<unsigned char> filtered(height * width);

    std::vector<std::thread> threads;
    int rowsPerThread = (height + numThreads - 1) / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int startY = i * rowsPerThread;
        int endY = std::min(startY + rowsPerThread, static_cast<int>(height));

        threads.emplace_back([&original, &filtered, startY, endY, width, height]() {
            gaussBlock(original.pic_data, filtered, startY, endY, width, height);
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    output.write(reinterpret_cast<char*>(filtered.data()), filtered.size());
    output.close();

    return true;
}

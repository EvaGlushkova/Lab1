/* Glushkova Eva st108255@student.spbu.ru
Lab1 */
#ifndef BMP_PARALLEL_H
#define BMP_PARALLEL_H

#include "../bmp/bmp_struct.h"
#include <vector>
/**
 * @class BMPParallelService
 * @brief Service for parallel image processing with multiple threads
 */
class BMPParallelService {
private:
    int numThreads;

    static void leftRotateBlock(const BMPfile& original, std::vector<unsigned char>& result,
                                int startY, int endY, int width, int height);

    static void rightRotateBlock(const BMPfile& original, std::vector<unsigned char>& result,
                                 int startY, int endY, int width, int height);

    static void gaussBlock(const std::vector<unsigned char>& input, std::vector<unsigned char>& output,
                           int startY, int endY, int width, int height);

public:
    /**
    * @brief Constructor for BMPParallelService
    * @param numThreads Number of threads to use
    */
    BMPParallelService(int numThreads);
    /**
    * @brief Rotates image 90 degrees left using parallel processing
    * @return True if successful
    */
    bool LeftClock90(BMPfile& original, const char* outputFile);
    /**
     * @brief Rotates image 90 degrees right using parallel processing
     * @return True if successful
     */
    bool RightClock90(BMPfile& original, const char* outputFile);
    /**
     * @brief Applies Gaussian filter using parallel processing
     * @return True if successful
     */
    bool Gauss(BMPfile& original, const char* outputFile);
};

#endif
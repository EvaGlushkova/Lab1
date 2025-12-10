#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <cstring>

#include "bmp/bmp_struct.h"
#include "bmp/bmp_file.h"
#include "parallel/bmp_parallel.h"

void printComparison(const std::vector<long>& singleThreadTimes,
                     const std::vector<long>& multiThreadTimes,
                     int numThreads) {
   
    std::cout << "Execution time comparison (ms):" << std::endl;
    std::cout << "Single thread | " << numThreads << " threads" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;

    const char* operations[] = {"Left rotation", "Right rotation", "Gaussian filter"};

    for (size_t i = 0; i < singleThreadTimes.size(); ++i) {
        std::cout << operations[i] << ": " << singleThreadTimes[i]
                  << " ms | " << multiThreadTimes[i] << " ms" << std::endl;
    }

}

int main() {
    const char my_file[] = "my_file.bmp";
    
    
    try {
        BMPfile bmp = loadfile(my_file);
        
        uint32_t width = bmp.BitMapInfo.width;
        uint32_t height = bmp.BitMapInfo.height;
        
        
        if (bmp.pic_data.size() != width * height) {
            std::cerr << "ERROR: Pixel data size doesn't match dimensions!" << std::endl;
            return 1;
        }
        
        std::vector<long> singleThreadTimes;
        std::vector<long> multiThreadTimes;

        const int NUM_THREADS = 12;

        BMPParallelService parallelService(NUM_THREADS);
        
        auto start = std::chrono::steady_clock::now();
        bool leftResult = LeftClock90(bmp, "left90Clock.bmp");
        auto end = std::chrono::steady_clock::now();
        
        if (leftResult) {
            long time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            singleThreadTimes.push_back(time);
      
        } else {
            std::cerr << "Error LeftClock90" << std::endl;
            singleThreadTimes.push_back(0);
        }

        start = std::chrono::steady_clock::now();
        bool leftParallelResult = parallelService.LeftClock90(bmp, "left90Clock_parallel.bmp");
        end = std::chrono::steady_clock::now();
        
        if (leftParallelResult) {
            long time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            multiThreadTimes.push_back(time);
      
        } else {
            std::cerr << "Error LeftClock90 (parallel)" << std::endl;
            multiThreadTimes.push_back(0);
        }

        
        start = std::chrono::steady_clock::now();
        bool rightResult = RightClock90(bmp, "right90Clock.bmp");
        end = std::chrono::steady_clock::now();
        
        if (rightResult) {
            long time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            singleThreadTimes.push_back(time);
           
        } else {
            std::cerr << "Error RightClock90" << std::endl;
            singleThreadTimes.push_back(0);
        }

start = std::chrono::steady_clock::now();
        bool rightParallelResult = parallelService.RightClock90(bmp, "right90Clock_parallel.bmp");
        end = std::chrono::steady_clock::now();
        
        if (rightParallelResult) {
            long time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            multiThreadTimes.push_back(time);
          
        } else {
            std::cerr << "Error RightClock90 (parallel)" << std::endl;
            multiThreadTimes.push_back(0);
        }
        
        start = std::chrono::steady_clock::now();
        bool gaussResult = Gauss(bmp, "gaussian.bmp");
        end = std::chrono::steady_clock::now();
        
        if (gaussResult) {
            long time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            singleThreadTimes.push_back(time);
        } else {
            std::cerr << "Error Gauss" << std::endl;
            singleThreadTimes.push_back(0);
        }

        start = std::chrono::steady_clock::now();
        bool gaussParallelResult = parallelService.Gauss(bmp, "gaussian_parallel.bmp");
        end = std::chrono::steady_clock::now();
        
        if (gaussParallelResult) {
            long time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            multiThreadTimes.push_back(time);
           
        } else {
            std::cerr << "Error Gauss (parallel)" << std::endl;
            multiThreadTimes.push_back(0);
        }

        printComparison(singleThreadTimes, multiThreadTimes, NUM_THREADS);
        
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception caught" << std::endl;
        return 1;
    }

    return 0;
}

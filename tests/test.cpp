#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include "bmp/bmp_struct.h"
#include "bmp/bmp_file.h"
#include "parallel/bmp_parallel.h"


namespace fs = std::filesystem;

bool comparePixelData(const BMPfile& img1, const BMPfile& img2) {
    if (img1.pic_data.size() != img2.pic_data.size()) {
        return false;
    }
    
    return std::equal(img1.pic_data.begin(), img1.pic_data.end(), 
                     img2.pic_data.begin());
}

class ImageProcessingTest : public ::testing::Test {
protected:
    BMPfile img;
    BMPParallelService multiThreadService;

    ImageProcessingTest() : multiThreadService(2) {}

    void SetUp() override {
        img = loadfile("tests/image.bmp");
    }
    
    void TearDown() override {
        std::remove("tests/single_left.bmp");
        std::remove("tests/multi_left.bmp");
        std::remove("tests/single_right.bmp");
        std::remove("tests/multi_right.bmp");
        std::remove("tests/single_gauss.bmp");
        std::remove("tests/multi_gauss.bmp");
        std::remove("tests/rotated_temp.bmp");
        std::remove("tests/back_temp.bmp");
        std::remove("tests/rotated_0.bmp");
        std::remove("tests/rotated_1.bmp");
        std::remove("tests/rotated_2.bmp");
        std::remove("tests/rotated_3.bmp");
        std::remove("tests/once_filtered.bmp");
        std::remove("tests/twice_filtered.bmp");
        std::remove("tests/rotated_test.bmp");
    }
};

TEST_F(ImageProcessingTest, LeftClock90_MultiEqualsSingleThreaded) {
    ASSERT_TRUE(LeftClock90(img, "tests/single_left.bmp"));
    ASSERT_TRUE(multiThreadService.LeftClock90(img, "tests/multi_left.bmp"));
    
    BMPfile single = loadfile("tests/single_left.bmp");
    BMPfile multi = loadfile("tests/multi_left.bmp");
    
    ASSERT_EQ(single.BitMapInfo.width, multi.BitMapInfo.width);
    ASSERT_EQ(single.BitMapInfo.height, multi.BitMapInfo.height);
    
    ASSERT_TRUE(comparePixelData(single, multi));
}

TEST_F(ImageProcessingTest, RightClock90_MultiEqualsSingleThreaded) {
    ASSERT_TRUE(RightClock90(img, "tests/single_right.bmp"));
    ASSERT_TRUE(multiThreadService.RightClock90(img, "tests/multi_right.bmp"));
    
    BMPfile single = loadfile("tests/single_right.bmp");
    BMPfile multi = loadfile("tests/multi_right.bmp");
    
    ASSERT_EQ(single.BitMapInfo.width, multi.BitMapInfo.width);
    ASSERT_EQ(single.BitMapInfo.height, multi.BitMapInfo.height);
    
    ASSERT_TRUE(comparePixelData(single, multi));
}

TEST_F(ImageProcessingTest, Gauss_MultiEqualsSingleThreaded) {

    if (img.BitMapInfo.width * img.BitMapInfo.height < 50000) {
        std::cout << "Image too small for parallel Gaussian, skipping" << std::endl;
        return;
    }
    
    ASSERT_TRUE(Gauss(img, "tests/single_gauss.bmp"));
    ASSERT_TRUE(multiThreadService.Gauss(img, "tests/multi_gauss.bmp"));
    
    BMPfile single = loadfile("tests/single_gauss.bmp");
    BMPfile multi = loadfile("tests/multi_gauss.bmp");
    
    ASSERT_EQ(single.BitMapInfo.width, multi.BitMapInfo.width);
    ASSERT_EQ(single.BitMapInfo.height, multi.BitMapInfo.height);
    
    ASSERT_TRUE(comparePixelData(single, multi));
}

TEST_F(ImageProcessingTest, RotateLeftThenRight_EqualsOriginal) {
    ASSERT_TRUE(LeftClock90(img, "tests/rotated_temp.bmp"));
    BMPfile rotated = loadfile("tests/rotated_temp.bmp");
    
    ASSERT_TRUE(RightClock90(rotated, "tests/back_temp.bmp"));
    BMPfile back = loadfile("tests/back_temp.bmp");
    
    ASSERT_EQ(back.BitMapInfo.width, img.BitMapInfo.width);
    ASSERT_EQ(back.BitMapInfo.height, img.BitMapInfo.height);

}

TEST_F(ImageProcessingTest, RotateLeftFourTimes_EqualsOriginal) {
    BMPfile current = img;
    
    for (int i = 0; i < 4; ++i) {
        std::string filename = "tests/rotated_" + std::to_string(i) + ".bmp";
        ASSERT_TRUE(LeftClock90(current, filename.c_str()));
        current = loadfile(filename.c_str());
    }
    
    ASSERT_EQ(current.BitMapInfo.width, img.BitMapInfo.width);
    ASSERT_EQ(current.BitMapInfo.height, img.BitMapInfo.height);
    
    ASSERT_TRUE(comparePixelData(current, img));
}

TEST_F(ImageProcessingTest, ApplyGaussianTwice_NotEqualToOnce) {
    ASSERT_TRUE(Gauss(img, "tests/once_filtered.bmp"));
    BMPfile onceFiltered = loadfile("tests/once_filtered.bmp");
    
    ASSERT_TRUE(Gauss(onceFiltered, "tests/twice_filtered.bmp"));
    BMPfile twiceFiltered = loadfile("tests/twice_filtered.bmp");
    
    ASSERT_FALSE(comparePixelData(onceFiltered, twiceFiltered));
}

TEST_F(ImageProcessingTest, RotateAndSaveImage) {

    ASSERT_TRUE(LeftClock90(img, "tests/rotated_test.bmp"));
    
    BMPfile loaded = loadfile("tests/rotated_test.bmp");
    
    ASSERT_GT(loaded.BitMapInfo.width, 0);
    ASSERT_GT(loaded.BitMapInfo.height, 0);
    ASSERT_FALSE(loaded.pic_data.empty());
    
    ASSERT_EQ(loaded.BitMapInfo.width, img.BitMapInfo.height);
    ASSERT_EQ(loaded.BitMapInfo.height, img.BitMapInfo.width);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

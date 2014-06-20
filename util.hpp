#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>

#define GRAY_LEVEL 0
#define CURRENT_LEVEL_CNT 1
#define CALC_LEVEL_CNT 2
#define ARRAY_DIM 3

using namespace cv;
void timeMeasureStart(void);
void timeMeasureOver(const std::string& str);
void histogram_stat(const Mat& image, unsigned int gray[][ARRAY_DIM]);

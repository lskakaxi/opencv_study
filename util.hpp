#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
void timeMeasureStart(void);
void timeMeasureOver(const std::string& str);
void histogram_stat(const Mat& image, unsigned int gray[][2]);

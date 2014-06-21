#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "util.hpp"

using namespace cv;
using namespace std;

Mat Sobel_H_kern = (Mat_<char>(3, 3) << -1, -2, -1,
                                         0,  0,  0,
                                         1,  2,  1);
Mat Sobel_V_kern = (Mat_<char>(3, 3) << -1,  0,  1,
                                        -2,  0,  2,
                                        -1,  2,  1);
Mat LOG_kern = (Mat_<char>(5, 5) << -2, -4, -4, -4, -2,
                                    -4,  0,  8,  0, -4,
                                    -4,  8, 24,  8,-24,
                                    -4,  0,  8,  0, -4,
                                    -2, -4, -4, -4, -2);

void Edge_detect(const Mat& imgin, Mat& imgout, const Mat& kernel)
{
    cv::filter2D(imgin, imgout, imgin.depth(), kernel);
}

bool not_edge(uchar a, uchar b)
{
    if (abs(a - b) > 100)
        return false;
    return true;
}

void get_edge(Mat& img, Mat& out)
{
    uchar n1, n2, n3,
          n4, n5, n6,
          n7, n8, n9;

    out.create(img.size(), img.type());

    for (int i = 1; i < img.rows - 1; i++) {
        uchar *p_cur = img.ptr<uchar>(i);
        uchar *p_prev = img.ptr<uchar>(i - 1);
        uchar *p_next  = img.ptr<uchar>(i + 1);
        uchar *pout = out.ptr<uchar>(i);
        for (int j = 1; j < img.cols - 1; j++) {
            n1 = p_prev[j-1];
            n2 = p_prev[j];
            n3 = p_prev[j+1];
            n4 = p_cur[j-1];
            n5 = p_cur[j];
            n6 = p_cur[j+1];
            n7 = p_next[j-1];
            n8 = p_next[j];
            n9 = p_next[j+1];
            if (not_edge(n1,n5) && not_edge(n2,n5) && not_edge(n3,n5) &&
                not_edge(n4,n5) && not_edge(n6,n5) && not_edge(n7,n5) &&
                not_edge(n8,n5) && not_edge(n9,n5))
                pout[j] = 0;
            else
                pout[j] = 255;
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        cout << "too few paramters!\n";
        return 0;
    }
    
    char *imageName = argv[1];

    Mat image;
    image = imread(imageName, 1);
    if (!image.data) {
        cout << "Cannot read image!\n";
        return 0;
    }

    /* convert to gray image */
    Mat gray_img;
    timeMeasureStart();
    cvtColor(image, gray_img, CV_BGR2GRAY);
    timeMeasureOver("cvtColor");

    /* Edge detect by Sobel Vertical */
    Mat sobel_v_img;
    timeMeasureStart();
    Edge_detect(gray_img, sobel_v_img, Sobel_V_kern);
    timeMeasureOver("Edge detection by Sobel vertical kernel");
    /* Edge detect by Sobel Horizontal */
    Mat sobel_h_img;
    timeMeasureStart();
    Edge_detect(gray_img, sobel_h_img, Sobel_H_kern);
    timeMeasureOver("Edge detection by Sobel horizontal kernel");
    /* Edge detect by LOG */
    Mat LOG_img;
    timeMeasureStart();
    Edge_detect(gray_img, LOG_img, LOG_kern);
    timeMeasureOver("Edge detection by LOG kernel");

    /* get edge*/
    Mat edge;
    timeMeasureStart();
    get_edge(sobel_h_img, edge);
    timeMeasureOver("get edge");

    imshow(imageName, image);
    imshow("Gray image", gray_img);
    imshow("Sobel vertical", sobel_v_img);
    imshow("Sobel horizontal", sobel_h_img);
    imshow("LOG", LOG_img);
    imshow("EDGE", edge);

    waitKey(0);
    return 0;
}

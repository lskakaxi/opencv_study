#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "util.hpp"

using namespace cv;
using namespace std;

int comp(const void *p, const void *q)
{
    unsigned int *a = (unsigned int *)p, *b = (unsigned int *)q;
    a++; b++; // CURRENT_LEVEL_CNT
    a++; b++; // CALC_LEVEL_CNT
    return *(unsigned int*)a - *(unsigned int *)b;
}

void histogram_equalization(const Mat& imgin, Mat& imgout)
{
    CV_Assert(imgin.channels() == 1);
    unsigned int gray_array[256][ARRAY_DIM];
    float s[256];
    unsigned int total = imgin.cols * imgin.rows;
    uchar max = 255, min = 0;

    imgout.create(imgin.size(), imgin.type());
    histogram_stat(imgin, gray_array);
    while (!gray_array[min++]); min--;
    while (!gray_array[max--]); max++;
    for (int i = 0; i < 256; i++)
        s[i] = (float)gray_array[i][CALC_LEVEL_CNT] / (float)total;

    for (int i = 0; i < imgin.rows; i++) {
        const uchar *pin = imgin.ptr<uchar>(i);
        uchar *pout = imgout.ptr<uchar>(i);
        for (int j = 0; j < imgin.cols; j++)
            pout[j] = (uchar)(s[pin[j]] * (float)(max - min)) + min;
    }
    /*
    qsort(gray_array, 256, sizeof(gray_array[0]), comp);
    for (int i = 0; i < 256; i++)
        cout << total << ": " << gray_array[i][2] << ": " << s[i]
             << ": " << s[i] * (float)(max - min) << endl;
    */
}

void gray_with_kern(const Mat& imgin, Mat& imgout)
{

    int nRows = imgin.rows;
    int nCols = imgin.cols;
    int channels = imgin.channels();
    int i, j;
    uchar *p;
    const uchar *pin;

    imgout.create(nRows, nCols, imgin.depth());

    if (imgin.channels() != 3) {
        cout << "Wrong image format!\n";
        return;
    }
    for (i = 0; i < nRows; i++) {
        p = imgout.ptr<uchar>(i);
        pin = imgin.ptr<uchar>(i);
        for (j = 0; j < nCols; j++) {
            /* BGR to gray */
            float tmp = pin[j*channels] * 0.114 + pin[j*channels+1] * 0.587 + pin[j*channels+2] * 0.299;
            p[j] = p[j+1] = p[j+2] = (uchar) tmp;
        }
    }
}

void sharpen(const Mat& imgin, Mat& imgout)
{
    Mat sharpen_kern = (Mat_<char>(3, 3) << 0, -1, 0,
                                          -1,  5, -1,
                                           0, -1,  0);

    cv::filter2D(imgin, imgout, imgin.depth(), sharpen_kern);
}

void sharpen_myself(const Mat& imgin, Mat& imgout)
{
    CV_Assert(imgin.depth() == CV_8U);
    imgout.create(imgin.size(), imgin.type());

    int channels = imgin.channels();

    for (int j = 1; j < imgin.rows; j++) {
        const uchar *prev_row = imgin.ptr<uchar>(j - 1);
        const uchar *current_row = imgin.ptr<uchar>(j);
        const uchar *next_row = imgin.ptr<uchar>(j + 1);

        uchar *output = imgout.ptr<uchar>(j);
        for (int i = channels; i < imgin.cols * channels - channels; i++) {
            *output++ = saturate_cast<uchar>(5 * current_row[i] - prev_row[i] - next_row[i] -
                        current_row[i-channels] - current_row[i+channels]);
        }
    }
    imgout.row(0).setTo(Scalar(0));
    imgout.row(imgout.rows-1).setTo(Scalar(0));
    imgout.col(0).setTo(Scalar(0));
    imgout.col(imgout.cols-1).setTo(Scalar(0));
}

void reverse_color(Mat& img)
{
    const int channels = img.channels();

    switch (channels) {
    case 1:
        for (int i = 0; i < img.rows; i++)
            for (int j = 0; j < img.cols; j++)
                img.at<uchar>(i, j) += 0xff;
        break;
    case 3:
        uchar *p;
        for (int i = 0; i < img.rows; i++) {
            p = img.ptr<uchar>(i);
            for (int j = 0; j < img.cols * channels; j++)
            {
                p[j] = 255 - p[j];
                /*
                img.at<Vec3b>(i, j)[0] = 0xff - img.at<Vec3b>(i, j)[0];
                img.at<Vec3b>(i, j)[1] = 0xff - img.at<Vec3b>(i, j)[1];
                img.at<Vec3b>(i, j)[2] = 0xff - img.at<Vec3b>(i, j)[2];
                */
            }
        }
        break;
    default:
        cout << "Wrong channel number of the image!" << endl;
    }
}

int main(int argc, char **argv)
{
    double t = (double)getTickCount();
    Mat lookUpTable(1, 256, CV_8U);

    Mat m(2, 2, CV_8UC3, Scalar(0, 255, 0));
    if (argc < 2) {
        cout << "too few paramters!\n";
        cout << endl << m << endl;
        return 0;
    }
    
    uchar *p = lookUpTable.data;
    for (int i = 0; i < 256; i++) {
        if (i < 200)
            p[i] = 0;
        else if (i > 200)
            p[i] = 255;
        else
            p[i] = i;
    }

    char *imageName = argv[1];

    Mat image;
    image = imread(imageName, 1);
    if (!image.data) {
        cout << "Cannot read image!\n";
        return 0;
    }

    /* convert to gray image */
    Mat gray_img, gray_img2, gray_img3;
    timeMeasureStart();
    cvtColor(image, gray_img, CV_BGR2GRAY);
    timeMeasureOver("cvtColor");
    timeMeasureStart();
    gray_with_kern(image, gray_img2);
    timeMeasureOver("gray_with_kern");
    timeMeasureStart();
    histogram_equalization(gray_img, gray_img3);
    timeMeasureOver("histogram_equalization");

    /* convert with lookuptable method */
    Mat lut_img;
    timeMeasureStart();
    LUT(image, lookUpTable, lut_img);
    timeMeasureOver("LUT");

    /* reverse color */
    Mat reverse_img = image.clone();
    timeMeasureStart();
    reverse_color(reverse_img);
    timeMeasureOver("Reverse color");

    /* sharpen */
    Mat sharpen_img;
    timeMeasureStart();
//    sharpen_myself(image, sharpen_img);
    sharpen(image, sharpen_img);
    timeMeasureOver("Sharpen");

    imshow(imageName, image);
    imshow("Gray image", gray_img);
    imshow("Gray image 2", gray_img2);
    imshow("Gray image 3", gray_img3);
    imshow("LUT image", lut_img);
    imshow("reverse color", reverse_img);
    imshow("sharpen", sharpen_img);

    waitKey(0);
    return 0;
}

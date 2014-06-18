#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "util.hpp"

using namespace cv;
using namespace std;

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
    Mat gray_img;
    timeMeasureStart();
    cvtColor(image, gray_img, CV_BGR2GRAY);
    timeMeasureOver("cvtColor");

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
    sharpen_myself(image, sharpen_img);
//    sharpen(image, sharpen_img);
    timeMeasureOver("Sharpen");

    /*
    imwrite("gray.jpg", gray_img);
    imwrite("LUT.jpg", lut_img);
    namedWindow(imageName, CV_WINDOW_AUTOSIZE);
    namedWindow("Gray image", CV_WINDOW_AUTOSIZE);
    namedWindow("LUT image", CV_WINDOW_AUTOSIZE);
    namedWindow("reserve color", CV_WINDOW_AUTOSIZE);
    */
    imshow(imageName, image);
    imshow("Gray image", gray_img);
    imshow("LUT image", lut_img);
    imshow("reverse color", reverse_img);
    imshow("sharpen", sharpen_img);

    waitKey(0);
    return 0;
}

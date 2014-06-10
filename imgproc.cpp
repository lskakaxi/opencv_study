#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
// used in signle thread model
static double t1;
void timeMeasureStart(void)
{
    t1 = (double) getTickCount();
}

void timeMeasureOver(string str)
{
    t1 = ((double)getTickCount() - t1) / getTickFrequency();
    cout << str << "cost time: " << t1 * 1000 << " ms" << endl;
}

void sharpen(const Mat& imgin, Mat& imgout)
{
    CV_Assert(imgin.depth() == CV_8U);

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

    imwrite("gray.jpg", gray_img);
    imwrite("LUT.jpg", lut_img);
    namedWindow(imageName, CV_WINDOW_AUTOSIZE);
    namedWindow("Gray image", CV_WINDOW_AUTOSIZE);
    namedWindow("LUT image", CV_WINDOW_AUTOSIZE);
    imshow(imageName, image);
    imshow("Gray image", gray_img);
    imshow("LUT image", lut_img);

    waitKey(0);
    return 0;
}

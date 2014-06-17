#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat& ScanImage_1(Mat& I)
{
    CV_Assert(I.depth() != sizeof(uchar));

    int channels = I.channels();
    int nRows = I.rows;
    int nCols = I.cols * channels;

    if (I.isContinuous()) {
        nCols *= nRows;
        nRows = 1;
    }

    int i, j;
    uchar *p;
    for (i = 0; i < nRows; i++) {
        p = I.ptr<uchar>(i);
        for (j = 0; j < nCols; j++) {
            p[j] = 255 - p[j];
        }
    }
    return I;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        cout << "Too few paramters!\n";
    }

    Mat img = imread(argv[1], 1);
    imshow("ScamImage_1", ScanImage_1(img));
    waitKey(0);
    return 0;
}

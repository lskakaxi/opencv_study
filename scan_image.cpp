#include <iostream>
#include <opencv2/opencv.hpp>
#include "util.hpp"

using namespace std;
using namespace cv;

Mat& ScanImage_3(Mat& I)
{
    CV_Assert(I.depth() != sizeof(uchar));

    int channels = I.channels();
    uchar *p = I.data;

    cout << "start ptr: " << (void *)p << endl;
    for (int i = 0; i < I.rows; i++)
        for (int j = 0; j < I.step; j++)
            *(p++) = 255 - *p;

    return I;
}

Mat& ScanImage_2(Mat& I)
{
    CV_Assert(I.depth() != sizeof(uchar));

    int channels = I.channels();

    switch (channels) {
        case 1:
            {
                MatIterator_<uchar> it, end;
                for (it = I.begin<uchar>(), end = I.end<uchar>(); it != end; it ++) {
                    *it = 255 - *it;;
                    *it = *it > 0 ? *it : 0;
                }
                break;
            }
        case 3:
            {
                MatIterator_<Vec3b> it, end;
                for (it = I.begin<Vec3b>(), end = I.end<Vec3b>(); it != end; it ++) {
                    (*it)[0] = 255 - (*it)[0];
                    (*it)[0] = (*it)[0] > 0 ? (*it)[0] : 0;
                    (*it)[1] = 255 - (*it)[1];
                    (*it)[1] = (*it)[1] > 0 ? (*it)[1] : 0;
                    (*it)[2] = 255 - (*it)[2];
                    (*it)[2] = (*it)[2] > 0 ? (*it)[2] : 0;
                }
                break;
            }
        default:
            break;
    }

    return I;
}

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
    Mat img1 = img.clone();
    Mat img2 = img.clone();
    Mat img3 = img.clone();
    timeMeasureStart();
    ScanImage_1(img1);
    timeMeasureOver("scan 1");
    timeMeasureStart();
    ScanImage_2(img2);
    timeMeasureOver("scan 2");
    timeMeasureStart();
    ScanImage_3(img3);
    timeMeasureOver("scan 3");
    imshow("ScamImage_1", img1);
    imshow("ScamImage_2", img2);
    imshow("ScamImage_3", img3);
    waitKey(0);
    return 0;
}

#include "util.hpp"

using namespace std;
using namespace cv;
static double t1;
void timeMeasureStart(void)
{
    t1 = (double) getTickCount();
}

void timeMeasureOver(const std::string& str)
{
    t1 = ((double)getTickCount() - t1) / getTickFrequency();
    cout << str << " cost time: " << t1 * 1000 << " ms" << endl;
}

void histogram_stat(const Mat& image, unsigned int gray[][2])
{
    int channels = image.channels();
    int nRows = image.rows;
    int nCols = image.cols * channels;

    if (image.isContinuous()) {
        nCols *= nRows;
        nRows = 1;
    }

    int i, j;
    for (i = 0; i < 256; i++) {
        gray[i][0] = i;
        gray[i][1] = 0;
    }

    const uchar *p;
    for (i = 0; i < nRows; i++) {
        p = image.ptr<uchar>(i);
        for (j = 0; j < nCols; j++) {
            gray[p[j]][1]++;
        }
    }
}


#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "util.hpp"

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    /* ./blending img1 img2 alpha */
    if (argc < 4) {
        cout << "too few paramters!\n";
        return 0;
    }
    
    char *imageName1 = argv[1];
    char *imageName2 = argv[2];
    float alpha = 0.5;
    if (atof(argv[3]) > 0.0 && atof(argv[3]) < 1.0)
	alpha = atof(argv[3]);

    Mat image1, image2, dst;
    image1 = imread(imageName1, 1);
    image2 = imread(imageName2, 1);
    if (!image1.data || !image2.data) {
        cout << "Cannot read image!\n";
        return 0;
    }
    float beta = 1 - alpha;

    timeMeasureStart();
    addWeighted( image1, alpha, image2, beta, 0.0, dst);
    timeMeasureOver("add weighted ");

    imshow("origin1", image1);
    imshow("origin2", image2);
    imshow("blending", dst);
    waitKey(0);
    return 0;
}

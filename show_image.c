#include <opencv2/highgui/highgui_c.h>

int main(int argc, char **argv)
{
    IplImage* img = cvLoadImage(argv[1], CV_LOAD_IMAGE_ANYCOLOR);
    cvNamedWindow("opencv", CV_WINDOW_AUTOSIZE);
    cvShowImage("opencv", img);
    cvWaitKey(0);
    cvReleaseImage(&img);
    cvDestroyWindow("opencv");
    return 0;
}

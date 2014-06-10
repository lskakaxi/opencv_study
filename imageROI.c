#include <opencv2/highgui/highgui_c.h>

/* usage
 * ./imageROI /home/lskakaxi/Pictures/fengjing.jpg 800 500 200 200 100
 */
int main(int argc, char **argv)
{
    IplImage *src;
    if (argc == 7 && ((src = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR)) != 0))
    {
        int x = atoi(argv[2]);
        int y = atoi(argv[3]);
        int width = atoi(argv[4]);
        int height = atoi(argv[5]);
        int add = atoi(argv[6]);
        cvSetImageROI(src, cvRect(x, y, width, height));
        cvAddS(src, cvScalar(add,0,0,0), src, NULL);
        cvResetImageROI(src);
        cvNamedWindow("ROI_add", CV_WINDOW_AUTOSIZE);
	cvShowImage("ROI_add", src);
        cvWaitKey(0);
        return 0;
    }
}

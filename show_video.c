#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>

int g_slider_position;
CvCapture *g_capture;

void onTrackbarSlide(int pos)
{
    cvSetCaptureProperty(g_capture, CV_CAP_PROP_POS_FRAMES, pos);
}

int main(int argc, char **argv)
{
    IplImage* frame;
    IplImage *smooth_frame;
    cvNamedWindow("opencv", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("opencv_smooth", CV_WINDOW_AUTOSIZE);
    g_capture = cvCreateFileCapture(argv[1]);
    int frames = (int) cvGetCaptureProperty(g_capture, CV_CAP_PROP_FRAME_COUNT);
    if (frames)
        cvCreateTrackbar("Position", "opencv", &g_slider_position, frames, onTrackbarSlide);
    while (1) {
        frame = cvQueryFrame(g_capture);
        if (!frame) break;
        smooth_frame = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);
        cvSmooth(frame, smooth_frame, CV_GAUSSIAN, 3, 3, 0, 0);
        cvShowImage("opencv", frame);
        cvShowImage("opencv_smooth", smooth_frame);
        cvReleaseImage(&smooth_frame);
        char c = cvWaitKey(33);
        if (c == 27) break;
    }
    cvReleaseCapture(&g_capture);
    cvDestroyWindow("opencv");
    cvDestroyWindow("opencv_smooth");
    return 0;
}

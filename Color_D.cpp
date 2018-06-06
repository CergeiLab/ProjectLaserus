#include <opencv2\core\core.hpp>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv\cv.hpp>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

IplImage* image = 0;
IplImage* dst = 0;

IplImage* hsv = 0;
IplImage* h_plane = 0;
IplImage* s_plane = 0;
IplImage* v_plane = 0;

IplImage* h_post = 0;
IplImage* s_post = 0;
IplImage* v_post = 0;

IplImage* hsv_all = 0;

int Hmin = 0;
int Hmax = 256;

int Smin = 0;
int Smax = 256;

int Vmin = 0;
int Vmax = 256;

int HSVmax = 256;

void myTrackbarHmin(int pos) {
	Hmin = pos;
	cvInRangeS(h_plane, cvScalar(Hmin), cvScalar(Hmax), h_post);
}

void myTrackbarHmax(int pos) {
	Hmax = pos;
	cvInRangeS(h_plane, cvScalar(Hmin), cvScalar(Hmax), h_post);
}

void myTrackbarSmin(int pos) {
	Smin = pos;
	cvInRangeS(s_plane, cvScalar(Smin), cvScalar(Smax), s_post);
}

void myTrackbarSmax(int pos) {
	Smax = pos;
	cvInRangeS(s_plane, cvScalar(Smin), cvScalar(Smax), s_post);
}

void myTrackbarVmin(int pos) {
	Vmin = pos;
	cvInRangeS(v_plane, cvScalar(Vmin), cvScalar(Vmax), v_post);
}

void myTrackbarVmax(int pos) {
	Vmax = pos;
	cvInRangeS(v_plane, cvScalar(Vmin), cvScalar(Vmax), v_post);
}

int main(int argc, char* argv[])
{
	char* filename = argc == 2 ? argv[1] : "Picture 7.jpg";

	image = cvLoadImage(filename, 1);

	printf("[i] image: %s\n", filename);
	assert(image != 0);

	hsv = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 3);

	h_plane = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	s_plane = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	v_plane = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);

	h_post = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	s_post = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	v_post = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);

	cvCvtColor(image, hsv, CV_BGR2HSV);
	cvSplit(hsv, h_plane, s_plane, v_plane, 0);
	
	double framemin = 0;
	double framemax = 0;

	cvMinMaxLoc(h_plane, &(double)framemin, &(double)framemax);
	printf("[H] %f x %f\n", framemin, framemax);
	Hmin = static_cast<int>(framemin);
	Hmax = static_cast<int>(framemax);
	cvMinMaxLoc(s_plane, &(double)framemin, &(double)framemax);
	printf("[S] %f x %f\n", framemin, framemax);
	Smin = static_cast<int>(framemin);
	Smax = static_cast<int>(framemax);
	cvMinMaxLoc(v_plane, &(double)framemin, &(double)framemax);
	printf("[V] %f x %f\n", framemin, framemax);
	Vmin = static_cast<int>(framemin);
	Vmax = static_cast<int>(framemax);

	cvNamedWindow("original", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("H", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("S", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("V", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("H range", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("S range", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("V range", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("hsv and", CV_WINDOW_AUTOSIZE);

	cvCreateTrackbar("Hmin", "H post", &Hmin, HSVmax, myTrackbarHmin);
	cvCreateTrackbar("Hmax", "H post", &Hmax, HSVmax, myTrackbarHmax);
	cvCreateTrackbar("Smin", "S post", &Smin, HSVmax, myTrackbarSmin);
	cvCreateTrackbar("Smax", "S post", &Smax, HSVmax, myTrackbarSmax);
	cvCreateTrackbar("Vmin", "V post", &Vmin, HSVmax, myTrackbarVmin);
	cvCreateTrackbar("Vmax", "V post", &Vmax, HSVmax, myTrackbarVmax);

	/*if (image->width <1366 / 4 && image->height<768 / 2) {
		cvMoveWindow("original", 0, 0);
		cvMoveWindow("H", image->width + 10, 0);
		cvMoveWindow("S", (image->width + 10) * 2, 0);
		cvMoveWindow("V", (image->width + 10) * 3, 0);
		cvMoveWindow("hsv and", 0, image->height + 30);
		cvMoveWindow("H range", image->width + 10, image->height + 30);
		cvMoveWindow("S range", (image->width + 10) * 2, image->height + 30);
		cvMoveWindow("V range", (image->width + 10) * 3, image->height + 30);
	}*/

	while (true) {

		// показываем картинку
		cvShowImage("original", image);

		cvShowImage("H", h_plane);
		cvShowImage("S", s_plane);
		cvShowImage("V", v_plane);

		cvShowImage("H post", h_post);
		cvShowImage("S post", s_post);
		cvShowImage("V post", v_post);
 
		cvAnd(h_post, s_post, hsv_all);
		cvAnd(hsv_all, v_post, hsv_all);

		cvShowImage("hsv all", hsv_all);

		char c = cvWaitKey(33);
		if (c == 27) { 
			break;
		}
	}

	printf("\n[i] Results:\n");
	printf("[H] %d x %d\n", Hmin, Hmax);
	printf("[S] %d x %d\n", Smin, Smax);
	printf("[V] %d x %d\n", Vmin, Vmax);

	cvReleaseImage(&image);
	cvReleaseImage(&hsv);
	cvReleaseImage(&h_plane);
	cvReleaseImage(&s_plane);
	cvReleaseImage(&v_plane);
	cvReleaseImage(&h_post);
	cvReleaseImage(&s_post);
	cvReleaseImage(&v_post);
	cvReleaseImage(&hsv_all);
	
	cvDestroyAllWindows();
	return 0;

}
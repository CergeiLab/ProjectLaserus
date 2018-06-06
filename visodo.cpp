#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;
using namespace std;

int MAN()
{
	Mat scr, dst, color_dst;


}

int main(int argc, char* argv[])
{
	IplImage* src = 0;
	IplImage* dst = 0;
	IplImage* color_dst = 0;

	
	char* filename = argc >= 2 ? argv[1] : "Picture 7.jpg";
	
	src = cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE);

	if (!src) {
		printf("[!] Error: cant load image: %s \n", filename);
		return -1;
	}

	printf("[i] image: %s\n", filename);

	
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* lines = 0;
	int i = 0;


	dst = cvCreateImage(cvGetSize(src), 8, 1);
	color_dst = cvCreateImage(cvGetSize(src), 8, 3);

	
	cvCanny(src, dst, 50, 200, 3);

	
	cvCvtColor(dst, color_dst, CV_GRAY2BGR);

	
	lines = cvHoughLines2(dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 50, 50, 10);

	
	for (i = 0; i < lines->total; i++) 
	{
		CvPoint* line = (CvPoint*)cvGetSeqElem(lines, i);
		cvLine(color_dst, line[0], line[1], CV_RGB(255, 0, 0), 3, CV_AA, 0);
	}

	
	cvNamedWindow("Source", 1);
	cvShowImage("Source", src);

	cvNamedWindow("Hough", 1);
	cvShowImage("Hough", color_dst);

	
	cvWaitKey(0);

	
	cvReleaseMemStorage(&storage);
	cvReleaseImage(&src);
	cvReleaseImage(&dst);
	cvReleaseImage(&color_dst);
	cvDestroyAllWindows();
	return 0;
}
	
	


	/*
	VideoCapture cap(0);

	
	if (!cap.isOpened()) {
		cout << "Camera not opened!";
		return 0;
	}

	
	//cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT, 960);

	Mat frame;

	while (true) {
		cap >> frame;

		imshow("Video", frame);

		char c = cvWaitKey(33);

		if (c == 27) { 
			break;
		}
	}
	*/
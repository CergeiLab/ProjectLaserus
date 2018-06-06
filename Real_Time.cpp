#pragma comment (lib, "ws2_32.lib")

#include <opencv\cv.h>
#include <opencv2\core\utility.hpp>
#include <opencv2\core.hpp>
#include <opencv2\opencv.hpp>
#include <opencv\highgui.h>
#include <opencv2\calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv\cxmisc.h>
#include <opencv\cvaux.h>


#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <stdexcept>
#include <Windows.h>
#include <sstream>
#include <cstring>
#include <math.h>
#include <ctime>


using namespace std;
using namespace cv;

#define CAM 0

bool FindColorMask = true,
FilterMask = false,
Create_line = true,
Auto_mask = false;

cv::Mat frame, frame_HSV, frame_mask;
IplImage* image;
IplImage* dst;
IplImage* color_dst;
Mat histImg;

int Hl_Mask, Sl_Mask, Vl_Mask,
Hh_Mask, Sh_Mask, Vh_Mask;

int Hl1 = 126, Sl1 = 0, Vl1 = 203, Hh1 = 255, Sh1 = 255, Vh1 = 255;

int main()
{
	
	cv::VideoCapture cap(CAM);

	
	if (!cap.isOpened())
		return -1;

	
	/*if (!FindColorMask)
	{
		ifstream Mask("Mask.dat");
		if (Mask.is_open())
		{
			Mask >> Hl_Mask;
			Mask >> Sl_Mask;
			Mask >> Vl_Mask;
			Mask >> Hh_Mask;
			Mask >> Sh_Mask;
			Mask >> Vh_Mask;

			Mask.close();
		}
		else
			cout << " error" << endl;
	}*/

	while (true)
	{
		int c = cvWaitKey(1);

		
		//cap >> frame;
		
		frame = cv::imread("2.jpg", CV_LOAD_IMAGE_COLOR);

		
		cvtColor(frame, frame_HSV, CV_BGR2HSV);

		
		if (FindColorMask)
		{
			cvNamedWindow("HSV Tune", CV_WINDOW_FREERATIO); //CV_WINDOW_AUTOSIZE
			int Hl = cvCreateTrackbar("H_low", "HSV Tune", &Hl1, 255, NULL);
			int Hh = cvCreateTrackbar("H_hight", "HSV Tune", &Hh1, 255, NULL);
			int Sl = cvCreateTrackbar("S_low", "HSV Tune", &Sl1, 255, NULL);
			int Sh = cvCreateTrackbar("S_hight", "HSV Tune", &Sh1, 255, NULL);
			int Vl = cvCreateTrackbar("V_low", "HSV Tune", &Vl1, 255, NULL);
			int Vh = cvCreateTrackbar("V_hight", "HSV Tune", &Vh1, 255, NULL);

			inRange(frame_HSV, Scalar(Hl1, Sl1, Vl1), Scalar(Hh1, Sh1, Vh1), frame_mask);
		}
		
		else
		{
			inRange(frame_HSV, Scalar(Hl_Mask, Sl_Mask, Vl_Mask), Scalar(Hh_Mask, Sh_Mask, Vh_Mask), frame_mask);
		}

		/*if (c == 79 || c == 111) // O
		{
			if (FindColorMask)
			{
				ofstream Mask("Mask.dat", ios::out | ios::trunc);
				if (Mask.is_open())
				{
					Mask << Hl1 << " " << Sl1 << " " << Vl1 << endl << Hh1 << " " << Sh1 << " " << Vh1;
					Mask.close();
				}
				else
					cout << "[i]  Mask.dat not found." << endl;
			}
			else
				cout << "[i]  Fail. FindColorMask != true" << endl;

		}*/

		
		if (FilterMask)
		{
			
			erode(frame_mask, frame_mask, getStructuringElement(MORPH_RECT, Size(3, 3)));
			erode(frame_mask, frame_mask, getStructuringElement(MORPH_RECT, Size(3, 3)));

			
			dilate(frame_mask, frame_mask, getStructuringElement(MORPH_RECT, Size(8, 8)));
			dilate(frame_mask, frame_mask, getStructuringElement(MORPH_RECT, Size(8, 8)));

			
			//GaussianBlur( frame_mask, frame_mask, Size(9, 9), 2, 2 );

		}

		if (Create_line)// image входная картинка.
		{
			image = cvCreateImage(cvSize(frame_HSV.cols, frame_HSV.rows), 8, 3);
			IplImage ipltemp = frame;
			cvCopy(&ipltemp, image);

			CvMemStorage* storage = cvCreateMemStorage(0);
			CvSeq* lines = 0;
			int i = 0;

			dst = cvCreateImage(cvGetSize(image), 8, 1);
			color_dst = cvCreateImage(cvGetSize(image), 8, 3);

			cvCanny(image, dst, 50, 200, 3);

			cvCvtColor(dst, color_dst, CV_GRAY2BGR);

			lines = cvHoughLines2(dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 50, 50, 10);


			for (i = 0; i < lines->total; i++)
			{
				CvPoint* line = (CvPoint*)cvGetSeqElem(lines, i);
				cvLine(color_dst, line[0], line[1], CV_RGB(255, 0, 0), 3, 4, 0);//color_dst
			}
		}

		if (Auto_mask)
		{
			//inRange(frame_HSV, Scalar(Hl1, Sl1, Vl1), Scalar(Hh1, Sh1, Vh1), frame_mask);
			int hbins = 256, sbins = 256, vbins = 256;//
			int histSize[] = { hbins, sbins, vbins };

			float hranges[] = { 0, 256 };

			float sranges[] = { 0, 256 };
			float vranges[] = { 0, 256 };
			const float* ranges[] = { hranges, sranges, vranges };
			MatND hist;

			int channels[] = { 0, 1, 2 };

			calcHist(&frame_HSV, 1, channels, frame_mask,//Mat()
				hist, 2, histSize, ranges,
				true,
				false);

			double maxVal = 0;
			minMaxLoc(hist, 0, &maxVal, 0, 0);

			int scale = 10;
			histImg = Mat::zeros(sbins*scale, hbins*scale, CV_8UC3);

			for (int h = 0; h < hbins; h++)
				for (int s = 0; s < sbins; s++)
				{
					float binVal = hist.at<float>(h, s);
					int intensity = cvRound(binVal * 255 / maxVal);
					rectangle(histImg, Point(h*scale, s*scale), Point((h + 1)*scale - 1, (s + 1)*scale - 1), Scalar::all(intensity), CV_FILLED);
				}
		}
		

		cvNamedWindow("RAW", CV_WINDOW_FREERATIO);
		cv::imshow("RAW", frame_HSV);

		if (FindColorMask)
		{
			cvNamedWindow("MASK", CV_WINDOW_FREERATIO);
			cv::imshow("MASK", frame_mask);
		}

		if (Create_line)
		{
			cvNamedWindow("LINE", CV_WINDOW_FREERATIO);
			cvShowImage("LINE", image);

			cvNamedWindow("Hough", CV_WINDOW_FREERATIO);
			cvShowImage("Hough", color_dst);
		}
		
		if (Auto_mask)
		{
			cvNamedWindow("Mask", CV_WINDOW_FREERATIO);
			cv::imshow("Mask", frame_mask);

			cvNamedWindow("Auto_MASK", CV_WINDOW_FREERATIO);
			cv::imshow("Auto_MASK", histImg);//добавить входное изображение
		}
		
		if (c == 27)
		{
			break;
		}
			

	}
	
	cout << "runtime: " << clock() / 1000.0 << endl;
	system("PAUSE");
	return 0;
}
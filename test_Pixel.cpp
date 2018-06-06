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
#include "test_Pixel.h"


using namespace std;
using namespace cv;

#define CAM 0

Mat frame, frame_HSV, frame_Gray, Line;
Mat hsv = Mat(frame.cols, frame.rows, 8, 3);
vector<Mat> splitHSV = vector<Mat>();

int main()
{
	setlocale(LC_ALL, "Rus");
	cv::VideoCapture cap(CAM);


	if (!cap.isOpened())
		return -1;

	while (true)
	{
		int c = cvWaitKey(1);

		frame = cv::imread("8.jpg");


		cvtColor(frame, frame_HSV, CV_BGR2HSV);
		cvtColor(frame, frame_Gray, CV_BGR2GRAY);

		cvtColor(frame, hsv, CV_BGR2GRAY);
		split(hsv, splitHSV);

		int pix;
		int roi = 52;

		//resol 1280x720

		for (int y = 0; y < frame_Gray.rows; y++)
		{
			for (int x = 0; x < frame_Gray.cols; x++)
			{
				pix = (int) frame_Gray.at<uchar>(y, x);
				//cout << "(" << pix <<")";
				if (pix >= roi)
				{
					frame_Gray.at <uchar>(y, x) = 255;
					//cout << "(" << pix <<")";
				}
				if (pix < roi)
				{
					frame_Gray.at<uchar>(y, x) = 0;
				}


			}

		}
		Line = frame_Gray.clone();

		for (int y = 0; y < Line.rows; y++)
		{
			for (int x = 0; x < Line.cols; x++)
			{
				Line.at<uchar>(y, x) = 0;

			}
		}

		int arr = 0;
		int ind = 0;
		int filtr = 0;

		for (int y = 0; y < frame_Gray.rows; y++)
		{
			for (int x = 0; x < frame_Gray.cols; x++)
			{
				pix = (int)frame_Gray.at<uchar>(y, x);
				
				if (pix > 0)
				{
					ind = x;
					arr++;

					pix = (int)frame_Gray.at<uchar>(y, x + 1);
					if (pix == 0)
					{
						y++;
						y--;
					}
				}

				if (x == (Line.cols-1)) 
				{
					filtr = ind + (arr / 2);
					Line.at<uchar>(y, filtr) = 255;
					Line.at<uchar>(y, filtr-1) = 255;
					Line.at<uchar>(y, filtr+1) = 255;
					
				}
				

			}
			
			//cout << "Строка:" << y << " число pix: " << arr << " первый инднкс: " << ind << " последний индекс: " << ind + arr
			//	<< " Разница: " << arr/2 << " фильтр: " << filtr << endl;
			arr = 0;
		}
		

		//cvNamedWindow("frame_HSV", CV_WINDOW_FREERATIO);
		//cv::imshow("frame_HSV", frame_HSV);

		cvNamedWindow("GRAY", CV_WINDOW_FREERATIO);
		cv::imshow("GRAY", frame_Gray);

		cvNamedWindow("HSV", CV_WINDOW_FREERATIO);
		cv::imshow("HSV", frame_HSV);

		cvNamedWindow("RAW", CV_WINDOW_FREERATIO);
		cv::imshow("RAW", frame);

		cvNamedWindow("Line", CV_WINDOW_FREERATIO);
		cv::imshow("Line", Line);


		if (c == 27)
		{
			break;
		}
		//system("PAUSE");
	}

	
	return 0;
}
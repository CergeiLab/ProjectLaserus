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

//Desctop_1
using namespace std;
using namespace cv;

#define CAM 0
//double PI = 3.141592653589793238462643383279;
double D;
double h;
double pfc;
void res_dist(double& _D, double& _h, double& _pcf)
{
	double x[8] = { 180, 160, 140, 120, 100, 80, 60, 40 };//сантиметры
	double y[8] = { 9, 15, 21, 31, 43, 62, 93, 157 };//пиксели
	double AY[8];
	double Theta;
	//double D;
	int i;
	double a, b;

	for (int j = 0; j<8; j++)
	{
		AY[j] = atan(_h / x[j]);

	}

	i = 0;
	while (i < 8 && y[i] <= y[7])//подправить услови€ дл€ реальных измерений
	{
		i++;

		a = (AY[i - 1] - AY[i]) / (y[i - 1] - y[i]);
		b = AY[i - 1] - a*y[i - 1];

		Theta = a*_pcf + b;
		_D = _h / tan(Theta);
		cout << "X: " << _pcf << " Y: " << Theta << " i: " << i << " Range:" << _D << endl;
	}

}

bool FindColorMask = true;
bool Histogramm = false;
bool MSK_2 = true;
bool Histogramm_1CHANNEL = true;
bool All_roi = true;

cv::Mat frame, frame_HSV, frame_mask, frame_mask2;
cv::Mat Line, All_Roi_mask;
Mat img, hsvChannels[3], bHist, gHist, rHist, HistImg;

int Hl_Mask, Sl_Mask, Vl_Mask,
Hh_Mask, Sh_Mask, Vh_Mask, Bl_Mask, Bh_Mask;

int pix, x;
double R_pix;
int pix_1;
int roi = 130;

int Hl1 = 123, Sl1 = 0, Vl1 = 86, Hh1 = 360, Sh1 = 255, Vh1 = 255, Bl1 = 0, Bh1 = 0;//int Hl1 = 126, Sl1 = 0, Vl1 = 203, Hh1 = 255, Sh1 = 255, Vh1 = 255;

int main()
{
	setlocale(LC_ALL, "Rus");
	//unsigned int start_time = clock();
	//unsigned int end_time;
	cv::VideoCapture cap(CAM);


	if (!cap.isOpened())
		return -1;



	while (true)
	{
		int c = cvWaitKey(1);


		//cap >> frame;

		frame = cv::imread("8.jpg", CV_LOAD_IMAGE_COLOR);


		cvtColor(frame, frame_HSV, CV_BGR2HSV);
		//cvtColor(frame, frame_GRAY, CV_BGR2GRAY);

		/*if (Histogramm)
		{

		int kBins = 256;
		float range[] = { 0.0f, 256.0f };
		const float* histRange = { range };

		bool unform = true;
		bool accumulate = false;

		int histWidth = 512, histHeight = 400;
		int binWidth = cvRound((double)histWidth / kBins);
		int i = 1, kChannels = 3;
		Scalar colors[] = { Scalar(255,0,0),Scalar(0,255,0),Scalar(0,0,255) };

		img = frame_HSV;
		split(img, hsvChannels);
		calcHist(&hsvChannels[0], 1, 0, Mat(), bHist, 1, &kBins, &histRange, unform, accumulate);
		calcHist(&hsvChannels[1], 1, 0, Mat(), gHist, 1, &kBins, &histRange, unform, accumulate);
		calcHist(&hsvChannels[2], 1, 0, Mat(), rHist, 1, &kBins, &histRange, unform, accumulate);

		HistImg = Mat(histHeight, histWidth, CV_8UC3, Scalar(0, 0, 0));

		normalize(bHist, bHist, 0, HistImg.rows, NORM_MINMAX, -1, Mat());
		normalize(gHist, gHist, 0, HistImg.rows, NORM_MINMAX, -1, Mat());
		normalize(rHist, rHist, 0, HistImg.rows, NORM_MINMAX, -1, Mat());

		for (i; i < kBins; i++)
		{
		line(HistImg, Point(binWidth*(i - 1), histHeight - cvRound(bHist.at<float>(i - 1))), Point(binWidth*i, histHeight - cvRound(bHist.at<float>(i))), colors[0], 2, 8, 0);
		line(HistImg, Point(binWidth*(i - 1), histHeight - cvRound(gHist.at<float>(i - 1))), Point(binWidth*i, histHeight - cvRound(gHist.at<float>(i))), colors[1], 2, 8, 0);
		line(HistImg, Point(binWidth*(i - 1), histHeight - cvRound(rHist.at<float>(i - 1))), Point(binWidth*i, histHeight - cvRound(rHist.at<float>(i))), colors[2], 2, 8, 0);


		}


		}*/


		if (Histogramm_1CHANNEL)
		{
			int kBins = 1280;//256
			float range[] = { 0.0f, 1280.0f };
			const float* histRange = { range };

			bool unform = true;
			bool accumulate = false;

			int histWidth = 1280, histHeight = 720;//512 400
			int binWidth = cvRound((double)histWidth / kBins);
			int i = 1, kChannels = 1;
			Scalar colors[] = { Scalar(255,0,255) };
			//*

			//*
			img = frame_HSV;
			split(img, hsvChannels);
			calcHist(&hsvChannels[0], 1, 0, Mat(), rHist, 1, &kBins, &histRange, unform, accumulate);

			HistImg = Mat(histHeight, histWidth, CV_8UC1);

			normalize(rHist, rHist, 0, HistImg.rows, NORM_MINMAX, -1, Mat());

			for (i; i < kBins; i++)
			{
				line(HistImg, Point(binWidth*(i - 1), histHeight - cvRound(rHist.at<float>(i - 1))), Point(binWidth*i, histHeight - cvRound(rHist.at<float>(i))), colors[1], 2, 8, 0);


			}

		}
		//***
		/*float pix1;
		int h = rHist.rows;
		int w = rHist.cols;
		for (int x = 0; x < h; x++)
		{
		for (int y = 0; y < w; y++)
		{
		pix1 = (float)rHist.at<float>(x, y);
		cout << "Y: " << y << "X: " << x << " (" << pix1 << ")" << endl;
		}
		}*/
		//***
		if (FindColorMask)
		{
			cvNamedWindow("HSV Tune", CV_WINDOW_FREERATIO); //CV_WINDOW_AUTOSIZE
			int Hl = cvCreateTrackbar("H_low", "HSV Tune", &Hl1, 255, NULL);
			int Hh = cvCreateTrackbar("H_hight", "HSV Tune", &Hh1, 255, NULL);
			int Sl = cvCreateTrackbar("S_low", "HSV Tune", &Sl1, 255, NULL);
			int Sh = cvCreateTrackbar("S_hight", "HSV Tune", &Sh1, 255, NULL);
			int Vl = cvCreateTrackbar("V_low", "HSV Tune", &Vl1, 255, NULL);
			int Vh = cvCreateTrackbar("V_hight", "HSV Tune", &Vh1, 255, NULL);

			int Bl = cvCreateTrackbar("Brigth_low", "HSV Tune", &roi, 255, NULL);
			int Bh = cvCreateTrackbar("Brigth_hight", "HSV Tune", &roi, 255, NULL);

			inRange(frame_HSV, Scalar(Hl1, Sl1, Vl1), Scalar(Hh1, Sh1, Vh1), frame_mask);
			//inRange(hsvChannels[2], Scalar(Bl1), Scalar(Bh1), frame_mask2);
		}

		else
		{
			inRange(frame_HSV, Scalar(Hl_Mask, Sl_Mask, Vl_Mask), Scalar(Hh_Mask, Sh_Mask, Vh_Mask), frame_mask);
			//inRange(hsvChannels[2], Scalar(Bl_Mask), Scalar(Bh_Mask), frame_mask2);
		}



		if (MSK_2)
		{
			Line = hsvChannels[2].clone();

			for (int i = 0; i < Line.rows; i++)
			{
				for (int j = 0; j < Line.cols; j++)
				{
					pix = (int)Line.at<uchar>(i, j);

					if (pix >= roi)
					{
						Line.at <uchar>(i, j) = 255;
					}
					if (pix < roi)
					{
						Line.at<uchar>(i, j) = 0;
					}
				}


			}
		}

		if (All_roi)
		{
			All_Roi_mask = Line.clone();
			All_Roi_mask = Scalar::all(0);

			for (int i = 0; i < All_Roi_mask.rows; i++)
			{
				for (int j = 0; j < All_Roi_mask.cols; j++)
				{
					pix = (int)Line.at<uchar>(i, j);
					pix_1 = (int)frame_mask.at<uchar>(i, j);

					if ((pix_1 >= 200) && (pix >= 200))
					{
						All_Roi_mask.at<uchar>(i, j) = 255;
					}

				}

			}
			//ѕодсчет пикселей
			for (int i = 0; i < All_Roi_mask.rows; i++)
			{
				for (int j = 0; j < All_Roi_mask.cols; j++)
				{
					x = All_Roi_mask.at<uchar>(i, j);
					if (x >= 200) // искомое значение пиксел€
					{
						if (j < (All_Roi_mask.cols / 2))
						{
							R_pix = (All_Roi_mask.cols / 2) - j;
							//res_dist(D, h, R_pix);//передача пикселей в расчет

							cout << "ќт центра<<: " << R_pix << endl;
						}
						if (j >(All_Roi_mask.cols / 2))
						{
							R_pix = j - (All_Roi_mask.cols / 2);
							//res_dist(D,h,R_pix);//передача пикселей в расчет

							cout << "ќт центра>>: " << R_pix << endl;
						}
					}
				}
			}


		}


		cvNamedWindow("RAW", CV_WINDOW_FREERATIO);
		cv::imshow("RAW", frame_HSV);

		cvNamedWindow("HSV to GRAY", CV_WINDOW_FREERATIO);
		cv::imshow("HSV to GRAY", hsvChannels[2]);

		//cvNamedWindow("hsvChannels[1]", CV_WINDOW_FREERATIO);
		//cv::imshow("hsvChannels[1]", hsvChannels[1]);

		//cvNamedWindow("hsvChannels[0]", CV_WINDOW_FREERATIO);
		//cv::imshow("hsvChannels[0]", hsvChannels[0]);

		cvNamedWindow("All_Roi_mask", CV_WINDOW_FREERATIO);
		cv::imshow("All_Roi_mask", All_Roi_mask);

		//cvNamedWindow("Bright", CV_WINDOW_FREERATIO);
		//cv::imshow("Bright", frame_mask2);

		if (FindColorMask)
		{
			cvNamedWindow("MASK", CV_WINDOW_FREERATIO);
			cv::imshow("MASK", frame_mask);
		}

		if (Histogramm)
		{
			cvNamedWindow("Histogramm", CV_WINDOW_FREERATIO);
			cv::imshow("Histogramm", HistImg);

		}

		if (Histogramm_1CHANNEL)
		{
			cvNamedWindow("Histogramm", CV_WINDOW_FREERATIO);
			cv::imshow("Histogramm", HistImg);
		}

		if (MSK_2)
		{
			cvNamedWindow("Line", CV_WINDOW_FREERATIO);
			cv::imshow("Line", Line);
		}

		//rows = frame_mask.rows;
		//cols = frame_mask.cols;
		//cout << rows << " " << cols << endl;
		if (c == 27)
		{
			break;
		}



	}

	//end_time = clock();

	//cout << "Time: " << end_time - start_time << endl;
	//system("PAUSE");
	return 0;
}
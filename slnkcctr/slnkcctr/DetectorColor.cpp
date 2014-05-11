#include "stdafx.h"

#include "DetectorColor.h"

// cv::
#include <opencv2/highgui/highgui.hpp> // namedWindow, CV_WINDOW_AUTOSIZE, imshow
#include <opencv2/imgproc/imgproc.hpp> // cvtColor, COLOR_BGR2HSV

static const char * const ID_SLINKY_0 = "slinky0";
static const char * const ID_SLINKY_1 = "slinky1";
static const char * const ID_HAND_LEFT = "handLeft";
static const char * const ID_HAND_RIGHT = "handRight";

static const int RADIUS = 1;

DetectorColor::DetectorColor(const char * const window)
: window(window), iLowH(0), iHighH(179), iLowS(0), iHighS(255), iLowV(0), iHighV(255) {
	cv::namedWindow(window, cv::WINDOW_NORMAL);

	cv::namedWindow("Control", cv::WINDOW_AUTOSIZE);
	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &iHighH, 179);
	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &iHighS, 255);
	cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &iHighV, 255);
}

FrameAnnotation
DetectorColor::detect(const cv::Mat& img) const {
	// TODO: Implement.
	FrameAnnotation annotation;
	FrameObject::Color color(0, 0, 255); // blue
	FrameObject::Pos pos(10, 20);
	annotation.insert(FrameObject(ID_SLINKY_0, pos, RADIUS, color));
	annotation.insert(FrameObject(ID_SLINKY_1, pos, RADIUS, color));

	cv::Mat imgHsv;
	cv::cvtColor(img, imgHsv, cv::COLOR_BGR2HSV);

	cv::Mat imgThresholded;
	cv::inRange(imgHsv, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), imgThresholded);

	cv::imshow(window, imgThresholded);

	return annotation;
}

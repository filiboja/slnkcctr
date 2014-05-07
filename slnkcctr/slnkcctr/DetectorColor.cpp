#include "stdafx.h"

#include "DetectorColor.hpp"

// cv::
#include <opencv2/highgui/highgui.hpp> // namedWindow, CV_WINDOW_AUTOSIZE, imshow
#include <opencv2/imgproc/imgproc.hpp> // cvtColor, COLOR_BGR2HSV

static const char * const ID_SLINKY_0 = "slinky0";
static const char * const ID_SLINKY_1 = "slinky1";
static const char * const ID_HAND_LEFT = "handLeft";
static const char * const ID_HAND_RIGHT = "handRight";

static const int RADIUS = 1;

DetectorColor::DetectorColor(const char * const window)
: window(window) {
	cv::namedWindow(window, CV_WINDOW_AUTOSIZE);
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
	cvtColor(img, imgHsv, cv::COLOR_BGR2HSV);

	cv::imshow(window, imgHsv);

	return annotation;
}

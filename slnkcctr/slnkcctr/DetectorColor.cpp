#include "stdafx.h"

#include "DetectorColor.h"

// std::
#include <iostream> // cout, endl

// cv::
#include <opencv2/core/core.hpp> // Mat, inRange
#include <opencv2/highgui/highgui.hpp>
// namedWindow, CV_WINDOW_NORMAL, imshow, resizeWindow, destroyWindow, createTrackbar
#include <opencv2/imgproc/imgproc.hpp> // Moments, moments

static void border(cv::Mat& img,
	const double& borderLeft = 0.0, const double& borderRight = 0.0,
	const double& borderTop = 0.0, const double& borderBottom = 0.0);
static void open(cv::Mat& img, const int& size);
static void close(cv::Mat& img, const int& size);

DetectorColor::DetectorColor(const std::string& id,
	const Limit& hueMin, const Limit& hueMax,
	const Limit& satMin, const Limit& satMax,
	const Limit& valMin, const Limit& valMax)
	: id(id), iLowH(hueMin), iHighH(hueMax), iLowS(satMin), iHighS(satMax), iLowV(valMin), iHighV(valMax),
	videoWinUse(false), limitsWinUse(false)
{}

DetectorColor::~DetectorColor() {
	if (videoWinUse) {
		cv::destroyWindow(videoWinName);
	}
	if (limitsWinUse) {
		cv::destroyWindow(limitsWinName);
	}
}

void
DetectorColor::enableWinVideo(const std::string& winname, const int& width, const int& height) {
	videoWinUse = true;
	videoWinName = winname;

	// Create window
	cv::namedWindow(videoWinName, cv::WINDOW_NORMAL);
	cv::resizeWindow(videoWinName, width, height);
}

void
DetectorColor::enableWinLimits(const std::string& winname) {
	limitsWinUse = true;
	limitsWinName = winname;

	// Create window
	cv::namedWindow(limitsWinName, cv::WINDOW_AUTOSIZE);

	// Create trackbars
	const char * const winnameChar = limitsWinName.c_str();
	cv::createTrackbar("Hue min", winnameChar, &iLowH, HUE_MAX); //Hue (0 - 179)
	cv::createTrackbar("Hue max", winnameChar, &iHighH, HUE_MAX);
	cv::createTrackbar("Saturation min", winnameChar, &iLowS, SAT_MAX); //Saturation (0 - 255)
	cv::createTrackbar("Saturation max", winnameChar, &iHighS, SAT_MAX);
	cv::createTrackbar("Value min", winnameChar, &iLowV, VAL_MAX); //Value (0 - 255)
	cv::createTrackbar("Value max", winnameChar, &iHighV, VAL_MAX);
}

DetectorColor::Pos
DetectorColor::detect(const cv::Mat& imgHsv, const cv::Mat& imgBgr) const {
	assert(imgHsv.channels() == 3);
	assert(imgHsv.size() == imgBgr.size());

	cv::Mat imgThresholded = threshold(imgHsv);

	border(imgThresholded, 0.25, 0.25, 0.0, 0.0);
	
	open(imgThresholded, 5);
	close(imgThresholded, 5);

	cv::Moments oMoments = cv::moments(imgThresholded);
	int x = (int)(oMoments.m10 / oMoments.m00);
	int y = (int)(oMoments.m01 / oMoments.m00);
	
	if (videoWinUse) {
		cv::Mat imgMasked;
		imgBgr.copyTo(imgMasked, imgThresholded); // apply mask `imgThresholded` to `imgBgr`
		cv::imshow(videoWinName, imgMasked);
	}

	return Pos(x, y);
}

cv::Mat
DetectorColor::threshold(const cv::Mat& imgHsv) const {
	const cv::Vec3b lowerb = cv::Vec3b(iLowH, iLowS, iLowV);
	const cv::Vec3b upperb = cv::Vec3b(iHighH, iHighS, iHighV);
	cv::Mat imgThresholded;
	cv::inRange(imgHsv, lowerb, upperb, imgThresholded);
	return imgThresholded;
}

static void border(cv::Mat& img,
	const double& borderLeft, const double& borderRight,
	const double& borderTop, const double& borderBottom)
{
	static const cv::Scalar_<uchar>& zeros = cv::Scalar_<uchar>::all(0);

	if (borderLeft > 0.0) {
		const int leftcol = int(borderLeft * img.cols);
		img.colRange(0, leftcol) = zeros;
	}
	if (borderRight > 0.0) {
		const int rightcol = int((1 - borderRight) * img.cols);
		img.colRange(rightcol, img.cols) = zeros;
	}
	if (borderTop > 0.0) {
		const int toprow = int(borderTop * img.rows);
		img.rowRange(0, toprow) = zeros;
	}
	if (borderBottom > 0.0) {
		const int bottomrow = int((1 - borderBottom) * img.rows);
		img.rowRange(bottomrow, img.rows) = zeros;
	}
}

static void open(cv::Mat& img, const int& size) {
	//morphological opening (removes small objects from the foreground)
	cv::erode(img, img, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(size, size)));
	cv::dilate(img, img, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(size, size)));
}

static void close(cv::Mat& img, const int& size) {
	//morphological closing (removes small holes from the foreground)
	cv::dilate(img, img, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(size, size)));
	cv::erode(img, img, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(size, size)));
}

bool
operator<(const DetectorColor& left, const DetectorColor& right) {
	return left.id < right.id;
}

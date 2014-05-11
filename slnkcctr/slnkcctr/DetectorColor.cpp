#include "stdafx.h"

#include "DetectorColor.h"

// cv::
#include <opencv2/core/core.hpp> // Mat, inRange
#include <opencv2/highgui/highgui.hpp>
// namedWindow, CV_WINDOW_NORMAL, imshow, resizeWindow, destroyWindow, createTrackbar

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
	const cv::Vec3b lowerb = cv::Vec3b(iLowH, iLowS, iLowV);
	const cv::Vec3b upperb = cv::Vec3b(iHighH, iHighS, iHighV);
	cv::Mat imgThresholded;
	cv::inRange(imgHsv, lowerb, upperb, imgThresholded);
	
	// TODO: Calculate result `Pos`.

	if (videoWinUse) {
		cv::Mat imgMasked;
		imgBgr.copyTo(imgMasked, imgThresholded);
		cv::imshow(videoWinName, imgMasked);
	}

	return Pos(0, 0);
}

#include "stdafx.h"

#include "DetectorColor.h"

// std::
#include <iostream> // cout, endl

// cv::
#include <opencv2/core/core.hpp> // Mat, inRange
#include <opencv2/highgui/highgui.hpp>
// namedWindow, CV_WINDOW_NORMAL, imshow, resizeWindow, destroyWindow, createTrackbar
#include <opencv2/imgproc/imgproc.hpp> // Moments, moments

static void open(cv::Mat& img, const int& size);
static void close(cv::Mat& img, const int& size);

DetectorColor::DetectorColor(const std::string& id, const cv::Size& imgSize,
	const Limit& hueMin, const Limit& hueMax,
	const Limit& satMin, const Limit& satMax,
	const Limit& valMin, const Limit& valMax)
	: id(id), imgSize(imgSize),
	iLowH(hueMin), iHighH(hueMax),
	iLowS(satMin), iHighS(satMax),
	iLowV(valMin), iHighV(valMax),
	cropX(0), cropWidth(imgSize.width),
	cropY(0), cropHeight(imgSize.height),
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
	cv::namedWindow(limitsWinName, cv::WINDOW_NORMAL);

	// Create trackbars
	const char * const winnameChar = limitsWinName.c_str();
	
	cv::createTrackbar("Hue min", winnameChar, &iLowH, HUE_MAX); //Hue (0 - 179)
	cv::createTrackbar("Hue max", winnameChar, &iHighH, HUE_MAX);
	cv::createTrackbar("Saturation min", winnameChar, &iLowS, SAT_MAX); //Saturation (0 - 255)
	cv::createTrackbar("Saturation max", winnameChar, &iHighS, SAT_MAX);
	cv::createTrackbar("Value min", winnameChar, &iLowV, VAL_MAX); //Value (0 - 255)
	cv::createTrackbar("Value max", winnameChar, &iHighV, VAL_MAX);

	// TODO: Add callback functions that ensure that the values define a valid roi at all times
	// and preferably update the other trackbars respectively.
	cv::createTrackbar("Crop X", winnameChar, &cropX, imgSize.width);
	cv::createTrackbar("Crop width", winnameChar, &cropWidth, imgSize.width);
	cv::createTrackbar("Crop Y", winnameChar, &cropY, imgSize.height);
	cv::createTrackbar("Crop height", winnameChar, &cropHeight, imgSize.height);
}

DetectorColor::Pos
DetectorColor::detect(const cv::Mat& imgHsv, const cv::Mat& imgBgr) const {
	assert(imgHsv.channels() == 3);
	assert(imgHsv.size() == imgBgr.size());

	const cv::Mat& imgCropped = crop(imgHsv);

	cv::Mat imgThresholded = threshold(imgCropped);

	open(imgThresholded, 2);
	close(imgThresholded, 2);

	cv::Moments oMoments = cv::moments(imgThresholded);
	int x = (int)(oMoments.m10 / oMoments.m00);
	int y = (int)(oMoments.m01 / oMoments.m00);
	// TODO: Report null instead of (0, 0) in case the object wasn't detected at all
	
	if (videoWinUse) {
		const cv::Mat& imgBgrCropped = crop(imgBgr);
		cv::Mat imgMasked;
		imgBgrCropped.copyTo(imgMasked, imgThresholded); // apply mask `imgThresholded` to `imgBgr`
		const Pos pos = Pos(x, y);
		const cv::Scalar color(0, 0, 255); // red
		cv::circle(imgMasked, pos, 4, color, 2);
		cv::imshow(videoWinName, imgMasked);
	}

	return Pos(cropX + x, cropY + y);
}

cv::Mat
DetectorColor::crop(const cv::Mat& img) const
{
	// Ensure that roi lies in the image and is non-empty in both dimensions
	if (cropX < 0) {
		cropX = 0;
	}
	if (cropX >= img.cols) {
		cropX = img.cols - 1;
	}
	if (cropX + cropWidth > img.cols) {
		cropWidth = img.cols - cropX;
	}
	if (cropWidth <= 0) {
		cropWidth = 1;
	}
	if (cropY < 0) {
		cropY = 0;
	}
	if (cropY >= img.rows) {
		cropY = img.rows - 1;
	}
	if (cropY + cropHeight > img.rows) {
		cropHeight = img.rows - cropY;
	}
	if (cropHeight <= 0) {
		cropHeight = 1;
	}

	const cv::Rect roi = cv::Rect(cropX, cropY, cropWidth, cropHeight);
	
	return img(roi);
}

cv::Mat
DetectorColor::threshold(const cv::Mat& imgHsv) const
{
	const cv::Vec3b lowerb = cv::Vec3b(iLowH, iLowS, iLowV);
	const cv::Vec3b upperb = cv::Vec3b(iHighH, iHighS, iHighV);
	cv::Mat imgThresholded;
	cv::inRange(imgHsv, lowerb, upperb, imgThresholded);
	return imgThresholded;
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

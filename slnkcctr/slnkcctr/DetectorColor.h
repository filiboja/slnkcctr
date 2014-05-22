// http://opencv-srf.blogspot.com/2010/09/object-detection-using-color-seperation.html

#pragma once

#include "stdafx.h"

// std::
#include <string> // string

// cv::
#include <opencv2/core/core.hpp> // Mat, Point2i, Size

#include "CropFilter.h"

class DetectorColor {
public:
	typedef cv::Point2i Pos;

	DetectorColor(const std::string& filename, const cv::Size& frameSize);
	~DetectorColor();
	
	// @param imgHsv image in HSV color space
	Pos detect(const cv::Mat& imgHsv, const cv::Mat& imgBgr) const;
private:
	cv::Mat threshold(const cv::Mat& imgHsv) const;

	CropFilter cropFilter;

	std::string name;
	cv::Size frameSize;

	// Limits
	typedef unsigned int ConfigLimit;
	typedef unsigned char Limit;
	Limit hueMin;
	Limit hueMax;
	Limit satMin;
	Limit satMax;
	Limit valMin;
	Limit valMax;

	// For trackbars
	int iLowH;
	int iHighH;
	int iLowS; 
	int iHighS;
	int iLowV;
	int iHighV;
	static void onTrackbarLimit(int, void * object);
	void updateLimits();

	bool windowVideoShow;
	bool windowLimitsShow;

	std::string windowVideoName() const;
	std::string windowLimitsName() const;

	void createWindowVideo() const;
	void createWindowLimits();

	static const Limit HUE_MIN = 0;
	static const Limit HUE_MAX = 179;
	static const Limit SAT_MIN = 0;
	static const Limit SAT_MAX = 255;
	static const Limit VAL_MIN = 0;
	static const Limit VAL_MAX = 255;
};

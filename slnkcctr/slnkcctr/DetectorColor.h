// http://opencv-srf.blogspot.com/2010/09/object-detection-using-color-seperation.html

#pragma once

#include "stdafx.h"

// std::
#include <string> // string

// cv::
#include <opencv2/core/core.hpp> // Mat, Point2i, Size

#include "CloseFilter.h"
#include "CropFilter.h"
#include "HsvFilter.h"
#include "OpenFilter.h"

class DetectorColor {
public:
	typedef cv::Point2i Pos;

	DetectorColor(const std::string& filename, const cv::Size& frameSize);
	~DetectorColor();
	
	// @param imgHsv image in HSV color space
	Pos detect(const cv::Mat& imgHsv, const cv::Mat& imgBgr) const;
private:
	CropFilter cropFilter;
	HsvFilter hsvFilter;
	OpenFilter openFilter;
	CloseFilter closeFilter;

	std::string name;
	cv::Size frameSize;

	bool windowVideoShow;
	bool windowLimitsShow;

	std::string windowVideoName() const;
	std::string windowLimitsName() const;

	void createWindowVideo() const;
	void createWindowLimits();
};

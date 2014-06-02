// http://opencv-srf.blogspot.com/2010/09/object-detection-using-color-seperation.html

#pragma once

#include "stdafx.h"

// std::
#include <stdexcept> // runtime_error
#include <string> // string

// cv::
#include <opencv2/core/core.hpp> // Mat, Point2i, Size

#include "CloseFilter.h"
#include "CropFilter.h"
#include "FramePos.h"
#include "HsvFilter.h"
#include "OpenFilter.h"

class DetectorColor {
public:
	typedef cv::Point2i Pos;
	typedef cv::Scalar Color;

	DetectorColor(const std::string& filename = std::string(), const cv::Size& frameSize = cv::Size(0, 0),
		const Color& color = Color(0, 0, 255));
	~DetectorColor();
	DetectorColor(const DetectorColor& other)
	{
		// Prevent copying to keep the windows alive
		throw std::runtime_error("DetectorColor(const DetectorColor&): Unsupported");
	};

	void init(const std::string& filename, const cv::Size& frameSize, const Color& color = Color(0, 0, 255));
	
	// @param imgHsv image in HSV color space
	FramePos detect(const cv::Mat& imgHsv, const cv::Mat& imgBgr) const;
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

	Color color;
};

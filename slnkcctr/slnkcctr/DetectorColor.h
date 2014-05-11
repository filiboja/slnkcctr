// http://opencv-srf.blogspot.com/2010/09/object-detection-using-color-seperation.html

#pragma once

#include "stdafx.h"

// std::
#include <string> // string

// cv::
#include <opencv2/core/core.hpp> // Mat, Point2i

class DetectorColor {
public:
	typedef int Limit;
	typedef cv::Point2i Pos;

	DetectorColor(const std::string& id,
		const Limit& hueMin = HUE_MIN, const Limit& hueMax = HUE_MAX,
		const Limit& satMin = SAT_MIN, const Limit& satMax = SAT_MAX,
		const Limit& valMin = VAL_MIN, const Limit& valMax = VAL_MAX);
	~DetectorColor();
	
	void enableWinVideo(const std::string& winname, const int& width = 640, const int& height = 480);
	void enableWinLimits(const std::string& winname);

	// @param imgHsv image in HSV color space
	Pos detect(const cv::Mat& imgHsv, const cv::Mat& imgBgr) const;

	friend bool operator<(const DetectorColor& left, const DetectorColor& right);
private:
	const std::string id;

	Limit iLowH;
	Limit iHighH;
	Limit iLowS; 
	Limit iHighS;
	Limit iLowV;
	Limit iHighV;

	bool videoWinUse;
	std::string videoWinName;

	bool limitsWinUse;
	std::string limitsWinName;

	static const Limit HUE_MIN = 0;
	static const Limit HUE_MAX = 179;
	static const Limit SAT_MIN = 0;
	static const Limit SAT_MAX = 255;
	static const Limit VAL_MIN = 0;
	static const Limit VAL_MAX = 255;
};

bool operator<(const DetectorColor& left, const DetectorColor& right);

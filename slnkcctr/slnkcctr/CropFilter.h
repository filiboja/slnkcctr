#pragma once

#include "stdafx.h"

// std::
#include <string> // string

// cv::
#include <opencv2/core/core.hpp> // Mat

class CropFilter {
public:
	typedef unsigned int CoordType;
	typedef cv::Rect_<CoordType> RoiType;

	CropFilter(const RoiType& roi);
	void createTrackbars(const std::string& winname, const CoordType& xMax, const CoordType& yMax);
	cv::Mat filter(const cv::Mat& img) const;
private:
	RoiType roi;

	// Trackbars
	typedef int TrackbarType;
	TrackbarType tbCropX;
	TrackbarType tbCropWidth;
	TrackbarType tbCropY;
	TrackbarType tbCropHeight;

	static void onChange(int, void * object);

	void update();
};

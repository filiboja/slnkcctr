#pragma once

#include "stdafx.h"

// std::
#include <string> // string

// cv::
#include <opencv2/core/core.hpp> // Mat

// morphological opening (removes small objects from the foreground)
class OpenFilter {
public:
	typedef int SizeType;

	OpenFilter(const SizeType& size);
	void createTrackbars(const std::string& winname, const SizeType& sizeMax);
	void filter(cv::Mat& img) const;
private:
	SizeType size;

	// Trackbar
	typedef int TrackbarType;
	TrackbarType tbSize;

	static void onChange(int, void * object);

	void update();
};

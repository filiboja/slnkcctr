#pragma once

#include "stdafx.h"

// std::
#include <string> // string

// cv::
#include <opencv2/core/core.hpp> // Mat

// morphological closing (removes small holes from the foreground)
class CloseFilter {
public:
	typedef int SizeType;

	CloseFilter(const SizeType& size);
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

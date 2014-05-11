// http://opencv-srf.blogspot.com/2010/09/object-detection-using-color-seperation.html

#pragma once

#include "stdafx.h"

// cv::
#include <opencv2/core/core.hpp> // Mat

#include "FrameAnnotation.h" // FrameAnnotation

class DetectorColor {
public:
	DetectorColor(const char * const window);
	FrameAnnotation detect(const cv::Mat& mat) const;
private:
	const char * const window;
	int iLowH;
	int iHighH;
	int iLowS; 
	int iHighS;
	int iLowV;
	int iHighV;
};

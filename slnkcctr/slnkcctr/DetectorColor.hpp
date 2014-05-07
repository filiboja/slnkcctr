// http://opencv-srf.blogspot.com/2010/09/object-detection-using-color-seperation.html

#pragma once

#include "stdafx.h"

// cv::
#include <opencv2/core/core.hpp> // Mat

#include "FrameAnnotation.hpp" // FrameAnnotation

class DetectorColor {
public:
	FrameAnnotation detect(const cv::Mat& mat) const;
};

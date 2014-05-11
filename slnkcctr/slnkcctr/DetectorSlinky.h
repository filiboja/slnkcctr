#pragma once

#include "stdafx.h"

// cv::
#include <opencv2/core/core.hpp> // Mat

#include "DetectorColor.h" // DetectorColor
#include "FrameAnnotation.h" // FrameAnnotation

class DetectorSlinky {
public:
	DetectorSlinky();
	FrameAnnotation detect(const cv::Mat& img) const;
private:
	DetectorColor detector;
};

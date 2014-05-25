#pragma once

#include "stdafx.h"

// std::
#include <string> // string

// cv::
#include <opencv2/core/core.hpp> // Mat

#include "DetectorColor.h" // DetectorColor
#include "FrameAnnotation.h" // FrameAnnotation

class DetectorSlinky {
public:
	DetectorSlinky(const cv::Size& imgSize, const std::string& config0 = "", const std::string& config1 = "");
	FrameAnnotation detect(const cv::Mat& img) const;
private:
	cv::Size imgSize;
	DetectorColor detector0;
	DetectorColor detector1;
};

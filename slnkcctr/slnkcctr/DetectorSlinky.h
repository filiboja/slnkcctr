#pragma once

#include "stdafx.h"

// std::
#include <string>
#include <vector>

// cv::
#include <opencv2/core/core.hpp> // Mat

#include "DetectorColor.h" // DetectorColor

class DetectorSlinky {
public:
	DetectorSlinky(const cv::Size& imgSize, const std::string& config0 = "", const std::string& config1 = "");
	std::vector<FramePos> detect(const cv::Mat& img) const;
private:
	cv::Size imgSize;

	static const size_t DETECTORS_NUM = 2;

	DetectorColor detectors[DETECTORS_NUM];
};

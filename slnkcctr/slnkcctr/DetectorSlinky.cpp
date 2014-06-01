#include "stdafx.h"

#include "DetectorSlinky.h"

// cv::
#include <opencv2/imgproc/imgproc.hpp> // cvtColor, COLOR_BGR2HSV

DetectorSlinky::DetectorSlinky(const cv::Size& imgSize, const std::string& config0, const std::string& config1)
	: imgSize(imgSize)
{
	DetectorColor detector0(config0, imgSize);
	DetectorColor detector1(config1, imgSize);
	
	detectors.push_back(detector0);
	detectors.push_back(detector1);
}

std::vector<FramePos>
DetectorSlinky::detect(const cv::Mat& img) const
{
	const cv::Size& originalSize = img.size();
	const double widthRatio = (double)originalSize.width / imgSize.width;
	const double heightRatio = (double)originalSize.height / imgSize.height;

	// Resize source image
	cv::Mat imgResized;
	cv::resize(img, imgResized, imgSize, 0.0, 0.0, cv::INTER_AREA);

	// Convert image to HSV color space
	cv::Mat imgHsv;
	cv::cvtColor(imgResized, imgHsv, cv::COLOR_BGR2HSV);

	std::vector<FramePos> result;
	for (Detectors::const_iterator it = detectors.begin(); it != detectors.end(); ++it) {
		FramePos pos = it->detect(imgHsv, imgResized); // Detect object
		pos.pos.x *= widthRatio;
		pos.pos.y *= heightRatio;
		result.push_back(pos);
	}

	return result;
}

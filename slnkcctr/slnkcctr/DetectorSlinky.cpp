#include "stdafx.h"

#include "DetectorSlinky.h"

// cv::
#include <opencv2/imgproc/imgproc.hpp> // cvtColor, COLOR_BGR2HSV

DetectorSlinky::DetectorSlinky(const cv::Size& imgSize, const std::string& config0, const std::string& config1)
	: imgSize(imgSize),
	detector0(config0, imgSize),
	detector1(config1, imgSize)
{}

FrameAnnotation
DetectorSlinky::detect(const cv::Mat& img) const {
	const cv::Size& originalSize = img.size();
	const double widthRatio = (double)originalSize.width / imgSize.width;
	const double heightRatio = (double)originalSize.height / imgSize.height;

	// Resize source image
	cv::Mat imgResized;
	cv::resize(img, imgResized, imgSize, 0.0, 0.0, cv::INTER_AREA);

	// Convert image to HSV color space
	cv::Mat imgHsv;
	cv::cvtColor(imgResized, imgHsv, cv::COLOR_BGR2HSV);

	FrameAnnotation annotation;

	{ // 0
		DetectorColor::Pos pos = detector0.detect(imgHsv, imgResized); // Detect object
		pos.x = (int)(pos.x * widthRatio);
		pos.y = (int)(pos.y * heightRatio);
		const std::string id = "slinky0";
		const int radius = 32;
		const FrameObject::Color color(255, 0, 0); // blue
		FrameObject frameObject(id, pos, radius, color);
		annotation.insert(frameObject);
	}
	{ // 1
		DetectorColor::Pos pos = detector1.detect(imgHsv, imgResized); // Detect object
		pos.x = (int)(pos.x * widthRatio);
		pos.y = (int)(pos.y * heightRatio);
		const std::string id = "slinky1";
		const int radius = 32;
		const FrameObject::Color color(0, 255, 0); // green
		FrameObject frameObject(id, pos, radius, color);
		annotation.insert(frameObject);
	}

	return annotation;
}

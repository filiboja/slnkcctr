#include "stdafx.h"

#include "DetectorSlinky.h"

// cv::
#include <opencv2/imgproc/imgproc.hpp> // cvtColor, COLOR_BGR2HSV

DetectorSlinky::DetectorSlinky() : detector("detector", 163, 179, 64, 255, 88, 255),
	detectorGreen("slinky_green", 32, 96, 69, 255, 0, 255)
{
	detector.enableWinVideo("video");
	detector.enableWinLimits("limits");
	detectorGreen.enableWinVideo("slinky_green/video");
	detectorGreen.enableWinLimits("slinky_green/limits");
}

DetectorSlinky::DetectorSlinky(const cv::Size& imgSize) : detector("detector", 163, 179, 64, 255, 88, 255),
	detectorGreen("slinky_green", 32, 96, 69, 255, 0, 255)
{
	detector.enableWinVideo("video", imgSize.width, imgSize.height);
	detector.enableWinLimits("limits");
	detectorGreen.enableWinVideo("slinky_green/video", imgSize.width, imgSize.height);
	detectorGreen.enableWinLimits("slinky_green/limits");
}

FrameAnnotation
DetectorSlinky::detect(const cv::Mat& img) const {
	// Convert `img` to HSV color space
	cv::Mat imgHsv;
	cv::cvtColor(img, imgHsv, cv::COLOR_BGR2HSV);

	FrameAnnotation annotation;

	{ // blue
		const DetectorColor::Pos pos = detector.detect(imgHsv, img); // Detect object
		const std::string id = "slinky_blue";
		const int radius = 32;
		const FrameObject::Color color(255, 0, 0); // blue
		FrameObject frameObject(id, pos, radius, color);
		annotation.insert(frameObject);
	}
	{ // green
		const DetectorColor::Pos pos = detectorGreen.detect(imgHsv, img); // Detect object
		const std::string id = "slinky_green";
		const int radius = 32;
		const FrameObject::Color color(0, 255, 0); // green
		FrameObject frameObject(id, pos, radius, color);
		annotation.insert(frameObject);
	}

	return annotation;
}

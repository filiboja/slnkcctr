#include "stdafx.h"

#include "DetectorSlinky.h"

// cv::
#include <opencv2/imgproc/imgproc.hpp> // cvtColor, COLOR_BGR2HSV

DetectorSlinky::DetectorSlinky() : detector("detector", 84, 104, 44, 92, 154, 255) {
	detector.enableWinVideo("video");
	detector.enableWinLimits("limits");
}

DetectorSlinky::DetectorSlinky(const cv::Size& imgSize) : detector("detector", 84, 104, 44, 92, 154, 255) {
	detector.enableWinVideo("video", imgSize.width, imgSize.height);
	detector.enableWinLimits("limits");
}

FrameAnnotation
DetectorSlinky::detect(const cv::Mat& img) const {
	// Convert `img` to HSV color space
	cv::Mat imgHsv;
	cv::cvtColor(img, imgHsv, cv::COLOR_BGR2HSV);

	FrameAnnotation annotation;
	DetectorColor::Pos pos = detector.detect(imgHsv, img); // Detect object
	const std::string id = "DetectColor";
	const int radius = 16;
	const FrameObject::Color color(255, 0, 0); // blue
	FrameObject frameObject(id, pos, radius, color);
	annotation.insert(frameObject);

	return annotation;
}

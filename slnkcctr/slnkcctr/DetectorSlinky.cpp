#include "stdafx.h"

#include "DetectorSlinky.h"

// cv::
#include <opencv2/imgproc/imgproc.hpp> // cvtColor, COLOR_BGR2HSV

DetectorSlinky::DetectorSlinky() : detector("detector") {
	detector.enableWinVideo("video");
	detector.enableWinLimits("limits");
}

FrameAnnotation
DetectorSlinky::detect(const cv::Mat& img) const {
	// Convert `img` to HSV color space
	cv::Mat imgHsv;
	cv::cvtColor(img, imgHsv, cv::COLOR_BGR2HSV);

	FrameAnnotation annotation;
	DetectorColor::Pos pos = detector.detect(imgHsv, img); // Detect object
	const char * const id = "DetectColor";
	const int radius = 1;
	const FrameObject::Color color(0, 0, 255); // blue
	FrameObject frameObject(id, pos, radius, color);
	annotation.insert(frameObject);

	return annotation;
}

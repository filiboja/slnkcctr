#include "stdafx.h"

#include "OpenFilter.h"

// cv::
#include <opencv2/highgui/highgui.hpp> // createTrackbar
#include <opencv2/imgproc/imgproc.hpp> // erode, dilate, ...

OpenFilter::OpenFilter(const SizeType& size) : size(size) {}

void OpenFilter::createTrackbars(const std::string& winname, const SizeType& sizeMax) {
	tbSize = size;

	cv::createTrackbar("Open size", winname, &tbSize, sizeMax, onChange, this);
}

void OpenFilter::filter(cv::Mat& img) const {
	cv::erode(img, img, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(size, size)));
	cv::dilate(img, img, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(size, size)));
}

void OpenFilter::onChange(int, void * object) {
	OpenFilter * filter = (OpenFilter *) object;
	filter->update();
}

void OpenFilter::update() {
	// TODO: Ensure validity.
	size = tbSize;
}

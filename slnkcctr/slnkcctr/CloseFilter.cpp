#include "stdafx.h"

#include "CloseFilter.h"

// cv::
#include <opencv2/highgui/highgui.hpp> // createTrackbar
#include <opencv2/imgproc/imgproc.hpp> // erode, dilate, ...

CloseFilter::CloseFilter(const SizeType& size) : size(size) {}

po::options_description CloseFilter::options() {
	// Set initial value
	configSize = 1;

	// Create options description
	po::options_description options;
	options.add_options()
		("morphological.close.size", po::value<ConfigType>(&configSize))
	;
	return options;
}

void CloseFilter::notify() {
	// TODO: Ensure validity.

	size = configSize;
}

void CloseFilter::createTrackbars(const std::string& winname, const SizeType& sizeMax) {
	tbSize = size;
	cv::createTrackbar("Close size", winname, &tbSize, sizeMax, onChange, this);
}

void CloseFilter::filter(cv::Mat& img) const {
	cv::dilate(img, img, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(size, size)));
	cv::erode(img, img, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(size, size)));
}

void CloseFilter::onChange(int, void * object) {
	CloseFilter * filter = (CloseFilter *) object;
	filter->update();
}

void CloseFilter::update() {
	// TODO: Ensure validity.
	size = tbSize;
}

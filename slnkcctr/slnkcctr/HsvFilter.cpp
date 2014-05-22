#include "stdafx.h"

#include "HsvFilter.h"

// cv::
#include <opencv2/highgui/highgui.hpp> // createTrackbar

HsvFilter::HsvFilter(const BoundaryType& lowerb, const BoundaryType& upperb)
	: lowerb(lowerb), upperb(upperb)
{}

po::options_description HsvFilter::options() {
	// Set initial values
	configHueMin = lowerb[B_HUE];
	configHueMax = upperb[B_HUE];
	configSatMin = lowerb[B_SAT];
	configSatMax = upperb[B_SAT];
	configValMin = lowerb[B_VAL];
	configValMax = upperb[B_VAL];

	// Create options description
	po::options_description options;
	options.add_options()
		("color.hue.min", po::value<ConfigType>(&configHueMin))
		("color.hue.max", po::value<ConfigType>(&configHueMax))
		("color.sat.min", po::value<ConfigType>(&configSatMin))
		("color.sat.max", po::value<ConfigType>(&configSatMax))
		("color.val.min", po::value<ConfigType>(&configValMin))
		("color.val.max", po::value<ConfigType>(&configValMax))
	;
	return options;
}

void HsvFilter::notify() {
	// TODO: Ensure validity.

	lowerb = BoundaryType(configHueMin, configSatMin, configValMin);
	upperb = BoundaryType(configHueMax, configSatMax, configValMax);
}

void HsvFilter::createTrackbars(const std::string& winname) {
	// Set initial values
	tbHueMin = lowerb[B_HUE];
	tbHueMax = upperb[B_HUE];
	tbSatMin = lowerb[B_SAT];
	tbSatMax = upperb[B_SAT];
	tbValMin = lowerb[B_VAL];
	tbValMax = upperb[B_VAL];

	// Create trackbars
	cv::createTrackbar("Hue min", winname, &tbHueMin, HUE_MAX, onChange, this);
	cv::createTrackbar("Hue max", winname, &tbHueMax, HUE_MAX, onChange, this);
	cv::createTrackbar("Sat min", winname, &tbSatMin, SAT_MAX, onChange, this);
	cv::createTrackbar("Sat max", winname, &tbSatMax, SAT_MAX, onChange, this);
	cv::createTrackbar("Val min", winname, &tbValMin, VAL_MAX, onChange, this);
	cv::createTrackbar("Val max", winname, &tbValMax, VAL_MAX, onChange, this);
}

void HsvFilter::filter(cv::Mat& img) const {
	// TODO: Wrap around 0 in hue.
	cv::inRange(img, lowerb, upperb, img);
}

void HsvFilter::onChange(int, void * object) {
	HsvFilter * filter = (HsvFilter *) object;
	filter->update();
}

void HsvFilter::update() {
	// TODO: Ensure validity.

	lowerb = BoundaryType(tbHueMin, tbSatMin, tbValMin);
	upperb = BoundaryType(tbHueMax, tbSatMax, tbValMax);
}

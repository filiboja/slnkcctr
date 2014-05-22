#pragma once

#include "stdafx.h"

// std::
#include <string> // string

// boost::
#include <boost/program_options.hpp>
namespace po = boost::program_options;

// cv::
#include <opencv2/core/core.hpp> // Mat

class HsvFilter {
public:
	typedef unsigned char ComponentType;
	typedef cv::Vec<ComponentType, 3> BoundaryType;

	HsvFilter(const BoundaryType& lowerb, const BoundaryType& upperb);
	po::options_description options();
	void notify();
	void createTrackbars(const std::string& winname);
	void filter(cv::Mat& img) const;
private:
	BoundaryType lowerb;
	BoundaryType upperb;

	static const ComponentType HUE_MAX = 179;
	static const ComponentType SAT_MAX = 255;
	static const ComponentType VAL_MAX = 255;

	enum {
		B_HUE = 0,
		B_SAT = 1,
		B_VAL = 2
	};

	// Trackbars
	typedef int TrackbarType;
	TrackbarType tbHueMin;
	TrackbarType tbHueMax;
	TrackbarType tbSatMin;
	TrackbarType tbSatMax;
	TrackbarType tbValMin;
	TrackbarType tbValMax;

	static void onChange(int, void * object);

	void update();

	// Configuration
	typedef unsigned int ConfigType;
	ConfigType configHueMin;
	ConfigType configHueMax;
	ConfigType configSatMin;
	ConfigType configSatMax;
	ConfigType configValMin;
	ConfigType configValMax;
};

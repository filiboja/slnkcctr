#include "stdafx.h"

#include "DetectorColor.h"

// std::
#include <iostream> // cout, endl

// cv::
#include <opencv2/core/core.hpp> // Mat, inRange
#include <opencv2/highgui/highgui.hpp>
// namedWindow, CV_WINDOW_NORMAL, imshow, resizeWindow, destroyWindow, createTrackbar
#include <opencv2/imgproc/imgproc.hpp> // Moments, moments

#include "program_options.h"

DetectorColor::DetectorColor(const std::string& filename, const cv::Size& frameSize)
	: frameSize(frameSize),
	windowVideoShow(false), windowLimitsShow(false),
	cropFilter(CropFilter::RoiType(0, 0, frameSize.width, frameSize.height)),
	hsvFilter(HsvFilter::BoundaryType(0, 0, 0), HsvFilter::BoundaryType(179, 255, 255)),
	openFilter(2), // TODO: Choose a sensible value.
	closeFilter(2) // TODO: Choose a sensible value.
{
	po::options_description options;
	options.add_options()
		("name", po::value<std::string>(&name))
		("window.video.show", po::value<bool>(&windowVideoShow)->default_value(windowVideoShow))
		("window.limits.show", po::value<bool>(&windowLimitsShow)->default_value(windowLimitsShow))
	;
	options.add(hsvFilter.options());
	po::variables_map vm;
	loadConfigFile(filename, options, vm);
	po::notify(vm);
	hsvFilter.notify();

	if (windowVideoShow) {
		createWindowVideo();
	}
	if (windowLimitsShow) {
		createWindowLimits();
	}
}

DetectorColor::~DetectorColor() {
	if (windowVideoShow) {
		cv::destroyWindow(windowVideoName());
	}
	if (windowLimitsShow) {
		cv::destroyWindow(windowLimitsName());
	}
}

void
DetectorColor::createWindowVideo() const {
	cv::namedWindow(windowVideoName(), cv::WINDOW_NORMAL);
	cv::resizeWindow(windowVideoName(), frameSize.width, frameSize.height);
}

void
DetectorColor::createWindowLimits() {
	std::string winname = windowLimitsName();

	// Create window
	cv::namedWindow(winname, cv::WINDOW_NORMAL);

	// Create trackbars
	cropFilter.createTrackbars(winname, frameSize.width, frameSize.height);
	hsvFilter.createTrackbars(winname);
	openFilter.createTrackbars(winname, 5); // TODO: Choose a sensible value.
	closeFilter.createTrackbars(winname, 5); // TODO: Choose a sensible value.
}

DetectorColor::Pos
DetectorColor::detect(const cv::Mat& imgHsv, const cv::Mat& imgBgr) const {
	assert(imgHsv.channels() == 3);
	assert(imgHsv.size() == imgBgr.size());

	cv::Mat imgCropped = cropFilter.filter(imgHsv);
	hsvFilter.filter(imgCropped); // in place
	cv::Mat& imgThresholded = imgCropped;

	openFilter.filter(imgThresholded);
	closeFilter.filter(imgThresholded);

	cv::Moments oMoments = cv::moments(imgThresholded);
	int x = (int)(oMoments.m10 / oMoments.m00);
	int y = (int)(oMoments.m01 / oMoments.m00);
	// TODO: Report null instead of (0, 0) in case the object wasn't detected at all
	
	if (windowVideoShow) {
		const cv::Mat imgBgrCropped = cropFilter.filter(imgBgr);
		cv::Mat imgMasked;
		imgBgrCropped.copyTo(imgMasked, imgThresholded); // apply mask `imgThresholded` to `imgBgr`
		const Pos pos = Pos(x, y);
		const cv::Scalar color(0, 0, 255); // red
		cv::circle(imgMasked, pos, 4, color, 2);
		cv::imshow(windowVideoName(), imgMasked);
	}

	return Pos(x, y);
}

std::string
DetectorColor::windowVideoName() const {
	return name + ".video";
}

std::string
DetectorColor::windowLimitsName() const {
	return name + ".limits";
}

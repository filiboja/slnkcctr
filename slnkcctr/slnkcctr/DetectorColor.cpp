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

DetectorColor::DetectorColor(const std::string& filename, const cv::Size& frameSize, const Color& color)
	: frameSize(frameSize), color(color),
	windowVideoShow(false), windowLimitsShow(false),
	cropFilter(CropFilter::RoiType(0, 0, frameSize.width, frameSize.height)),
	hsvFilter(),
	openFilter(),
	closeFilter()
{
	init(filename, frameSize, color, windowVideoShow, windowLimitsShow);
}

void DetectorColor::init(const std::string& filename, const cv::Size& frameSize, const Color& color,
	const bool& windowVideoShow, const bool& windowLimitsShow, const std::string& name)
{
	this->frameSize = frameSize;
	this->color = color;
	this->windowVideoShow = windowVideoShow;
	this->windowLimitsShow = windowLimitsShow;
	this->name = name;

	cropFilter.roi.x = 0;
	cropFilter.roi.y = 0;
	cropFilter.roi.width = frameSize.width;
	cropFilter.roi.height = frameSize.height;

	if (!filename.empty()) {
		po::options_description options;
		options.add_options()
			("name", po::value<std::string>(&this->name))
			("window.video.show", po::value<bool>(&this->windowVideoShow)->default_value(this->windowVideoShow))
			("window.limits.show", po::value<bool>(&this->windowLimitsShow)->default_value(this->windowLimitsShow))
		;
		options.add(hsvFilter.options());
		options.add(openFilter.options());
		options.add(closeFilter.options());
		po::variables_map vm;
		loadConfigFile(filename, options, vm);
		po::notify(vm);
		hsvFilter.notify();
		openFilter.notify();
		closeFilter.notify();
	}

	if (this->windowVideoShow) {
		createWindowVideo();
	}
	if (this->windowLimitsShow) {
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

FramePos
DetectorColor::detect(const cv::Mat& imgHsv, const cv::Mat& imgBgr) const {
	assert(imgHsv.channels() == 3);
	assert(imgHsv.size() == imgBgr.size());

	cv::Mat imgCropped = cropFilter.filter(imgHsv);
	hsvFilter.filter(imgCropped); // in place
	cv::Mat& imgThresholded = imgCropped;

	openFilter.filter(imgThresholded);
	closeFilter.filter(imgThresholded);

	FramePos result; // Invalid unless changed

	cv::Moments oMoments = cv::moments(imgThresholded);
	double area = oMoments.m00;
	if (area >= 0.5) {
		const double x = oMoments.m10 / oMoments.m00;
		const double y = oMoments.m01 / oMoments.m00;
		const cv::Point2d pos(x, y);
		result = FramePos(pos); // Make result a valid position
	}
	
	if (windowVideoShow) {
		const cv::Mat imgBgrCropped = cropFilter.filter(imgBgr);
		cv::Mat imgMasked;
		imgBgrCropped.copyTo(imgMasked, imgThresholded); // apply mask `imgThresholded` to `imgBgr`
		result.draw(imgMasked);
		cv::imshow(windowVideoName(), imgMasked);
	}

	result.color = color;

	return result;
}

std::string
DetectorColor::windowVideoName() const {
	return name + ".video";
}

std::string
DetectorColor::windowLimitsName() const {
	return name + ".limits";
}

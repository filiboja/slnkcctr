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

static void close(cv::Mat& img, const int& size);

DetectorColor::DetectorColor(const std::string& filename, const cv::Size& frameSize)
	: frameSize(frameSize),
	hueMin(HUE_MIN), hueMax(HUE_MAX), satMin(SAT_MIN), satMax(SAT_MAX), valMin(VAL_MIN), valMax(VAL_MAX),
	windowVideoShow(false), windowLimitsShow(false),
	cropFilter(CropFilter::RoiType(0, 0, frameSize.width, frameSize.height)),
	openFilter(2) // TODO: Choose a sensible value.
{
	ConfigLimit configHueMin = hueMin;
	ConfigLimit configHueMax = hueMax;
	ConfigLimit configSatMin = satMin;
	ConfigLimit configSatMax = satMax;
	ConfigLimit configValMin = valMin;
	ConfigLimit configValMax = valMax;

	po::options_description options;
	options.add_options()
		("name", po::value<std::string>(&name))
		("color.hue.min", po::value<ConfigLimit>(&configHueMin))
		("color.hue.max", po::value<ConfigLimit>(&configHueMax))
		("color.sat.min", po::value<ConfigLimit>(&configSatMin))
		("color.sat.max", po::value<ConfigLimit>(&configSatMax))
		("color.val.min", po::value<ConfigLimit>(&configValMin))
		("color.val.max", po::value<ConfigLimit>(&configValMax))
		("window.video.show", po::value<bool>(&windowVideoShow)->default_value(windowVideoShow))
		("window.limits.show", po::value<bool>(&windowLimitsShow)->default_value(windowLimitsShow))
	;
	po::variables_map vm;
	loadConfigFile(filename, options, vm);
	po::notify(vm);

	// TODO: Check for values out of bounds (crop and limits).

	hueMin = configHueMin;
	hueMax = configHueMax;
	satMin = configSatMin;
	satMax = configSatMax;
	valMin = configValMin;
	valMax = configValMax;

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
	const char * const winnameChar = winname.c_str();
	
	// Limits
	iLowH = hueMin;
	iHighH = hueMax;
	iLowS = satMin;
	iHighS = satMax;
	iLowV = valMin;
	iHighV = valMax;
	cv::createTrackbar("Hue min", winnameChar, &iLowH, HUE_MAX, onTrackbarLimit, this); // Hue (0 - 179)
	cv::createTrackbar("Hue max", winnameChar, &iHighH, HUE_MAX, onTrackbarLimit, this);
	cv::createTrackbar("Saturation min", winnameChar, &iLowS, SAT_MAX, onTrackbarLimit, this); // Saturation (0 - 255)
	cv::createTrackbar("Saturation max", winnameChar, &iHighS, SAT_MAX, onTrackbarLimit, this);
	cv::createTrackbar("Value min", winnameChar, &iLowV, VAL_MAX, onTrackbarLimit, this); // Value (0 - 255)
	cv::createTrackbar("Value max", winnameChar, &iHighV, VAL_MAX, onTrackbarLimit, this);

	cropFilter.createTrackbars(winname, frameSize.width, frameSize.height);
	openFilter.createTrackbars(winname, 5); // TODO: Choose a sensible value.
}

DetectorColor::Pos
DetectorColor::detect(const cv::Mat& imgHsv, const cv::Mat& imgBgr) const {
	assert(imgHsv.channels() == 3);
	assert(imgHsv.size() == imgBgr.size());

	const cv::Mat imgCropped = cropFilter.filter(imgHsv);
	cv::Mat imgThresholded = threshold(imgCropped);

	openFilter.filter(imgThresholded);
	close(imgThresholded, 2);

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

cv::Mat
DetectorColor::threshold(const cv::Mat& imgHsv) const
{
	const cv::Vec3b lowerb = cv::Vec3b(hueMin, satMin, valMin);
	const cv::Vec3b upperb = cv::Vec3b(hueMax, satMax, valMax);
	cv::Mat imgThresholded;
	cv::inRange(imgHsv, lowerb, upperb, imgThresholded);
	return imgThresholded;
}

static void close(cv::Mat& img, const int& size) {
	//morphological closing (removes small holes from the foreground)
	cv::dilate(img, img, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(size, size)));
	cv::erode(img, img, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(size, size)));
}

std::string
DetectorColor::windowVideoName() const {
	return name + ".video";
}

std::string
DetectorColor::windowLimitsName() const {
	return name + ".limits";
}

void DetectorColor::onTrackbarLimit(int, void * object) {
	DetectorColor * detector = (DetectorColor*) object;
	detector->updateLimits();
}

void DetectorColor::updateLimits() {
	hueMin = iLowH;
	hueMax = iHighH;
	satMin = iLowS;
	satMax = iHighS;
	valMin = iLowV;
	valMax = iHighV;
}

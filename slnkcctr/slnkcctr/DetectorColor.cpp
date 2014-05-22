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

static void open(cv::Mat& img, const int& size);
static void close(cv::Mat& img, const int& size);

DetectorColor::DetectorColor(const std::string& filename, const cv::Size& frameSize)
	: frameSize(frameSize),
	cropX(0), cropWidth(frameSize.width), cropY(0), cropHeight(frameSize.height),
	hueMin(HUE_MIN), hueMax(HUE_MAX), satMin(SAT_MIN), satMax(SAT_MAX), valMin(VAL_MIN), valMax(VAL_MAX),
	windowVideoShow(false), windowLimitsShow(false)
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
		("crop.x", po::value<Crop>(&cropX)->default_value(cropX))
		("crop.width", po::value<Crop>(&cropWidth)->default_value(cropWidth))
		("crop.y", po::value<Crop>(&cropY)->default_value(cropY))
		("crop.height", po::value<Crop>(&cropHeight)->default_value(cropHeight))
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

	// Crop
	iCropX = cropX;
	iCropWidth = cropWidth;
	iCropY = cropY;
	iCropHeight = cropHeight;
	cv::createTrackbar("Crop X", winnameChar, &iCropX, frameSize.width, onTrackbarCrop, this);
	cv::createTrackbar("Crop width", winnameChar, &iCropWidth, frameSize.width, onTrackbarCrop, this);
	cv::createTrackbar("Crop Y", winnameChar, &iCropY, frameSize.height, onTrackbarCrop, this);
	cv::createTrackbar("Crop height", winnameChar, &iCropHeight, frameSize.height, onTrackbarCrop, this);
}

DetectorColor::Pos
DetectorColor::detect(const cv::Mat& imgHsv, const cv::Mat& imgBgr) const {
	assert(imgHsv.channels() == 3);
	assert(imgHsv.size() == imgBgr.size());

	const cv::Mat& imgCropped = crop(imgHsv);

	cv::Mat imgThresholded = threshold(imgCropped);

	open(imgThresholded, 2);
	close(imgThresholded, 2);

	cv::Moments oMoments = cv::moments(imgThresholded);
	int x = (int)(oMoments.m10 / oMoments.m00);
	int y = (int)(oMoments.m01 / oMoments.m00);
	// TODO: Report null instead of (0, 0) in case the object wasn't detected at all
	
	if (windowVideoShow) {
		const cv::Mat& imgBgrCropped = crop(imgBgr);
		cv::Mat imgMasked;
		imgBgrCropped.copyTo(imgMasked, imgThresholded); // apply mask `imgThresholded` to `imgBgr`
		const Pos pos = Pos(x, y);
		const cv::Scalar color(0, 0, 255); // red
		cv::circle(imgMasked, pos, 4, color, 2);
		cv::imshow(windowVideoName(), imgMasked);
	}

	return Pos(cropX + x, cropY + y);
}

cv::Mat
DetectorColor::crop(const cv::Mat& img) const
{
	// Ensure that roi lies in the image and is non-empty in both dimensions
	/*if (cropX < 0) {
		cropX = 0;
	}
	if (cropX >= img.cols) {
		cropX = img.cols - 1;
	}
	if (cropX + cropWidth > img.cols) {
		cropWidth = img.cols - cropX;
	}
	if (cropWidth <= 0) {
		cropWidth = 1;
	}
	if (cropY < 0) {
		cropY = 0;
	}
	if (cropY >= img.rows) {
		cropY = img.rows - 1;
	}
	if (cropY + cropHeight > img.rows) {
		cropHeight = img.rows - cropY;
	}
	if (cropHeight <= 0) {
		cropHeight = 1;
	}*/
	assert(cropX >= 0);
	assert((int)cropX <= img.cols - 1);
	assert((int)(cropX + cropWidth) <= img.cols);
	assert(cropWidth > 0);
	assert(cropY >= 0);
	assert((int)cropY <= img.rows - 1);
	assert((int)(cropY + cropHeight) <= img.rows);
	assert(cropHeight > 0);

	const cv::Rect roi = cv::Rect(cropX, cropY, cropWidth, cropHeight);
	
	return img(roi);
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

static void open(cv::Mat& img, const int& size) {
	//morphological opening (removes small objects from the foreground)
	cv::erode(img, img, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(size, size)));
	cv::dilate(img, img, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(size, size)));
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
	detector->updateLimit();
}

void DetectorColor::updateLimit() {
	hueMin = iLowH;
	hueMax = iHighH;
	satMin = iLowS;
	satMax = iHighS;
	valMin = iLowV;
	valMax = iHighV;
}

void DetectorColor::onTrackbarCrop(int, void * object) {
	DetectorColor * detector = (DetectorColor*) object;
	detector->updateCrop();
}

void DetectorColor::updateCrop() {
	cropX = iCropX;
	cropWidth = iCropWidth;
	cropY = iCropY;
	cropHeight = iCropHeight;
}

#include "stdafx.h"

#include "CropFilter.h"

// cv::
#include <opencv2/highgui/highgui.hpp> // createTrackbar

CropFilter::CropFilter(const RoiType& roi) : roi(roi) {}

void CropFilter::createTrackbars(const std::string& winname, const CoordType& xMax, const CoordType& yMax) {
	tbCropX = roi.x;
	tbCropWidth = roi.width;
	tbCropY = roi.y;
	tbCropHeight = roi.height;

	cv::createTrackbar("Crop X", winname, &tbCropX, xMax, onChange, this);
	cv::createTrackbar("Crop width", winname, &tbCropWidth, xMax, onChange, this);
	cv::createTrackbar("Crop Y", winname, &tbCropY, yMax, onChange, this);
	cv::createTrackbar("Crop height", winname, &tbCropHeight, yMax, onChange, this);
}

cv::Mat CropFilter::filter(const cv::Mat& img) const {
	return img(roi);
}

void CropFilter::onChange(int, void * object) {
	CropFilter * filter = (CropFilter *) object;
	filter->update();
}

void CropFilter::update() {
	// TODO: Ensure validity.

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
	
	/*assert(cropX >= 0);
	assert((int)cropX <= img.cols - 1);
	assert((int)(cropX + cropWidth) <= img.cols);
	assert(cropWidth > 0);
	assert(cropY >= 0);
	assert((int)cropY <= img.rows - 1);
	assert((int)(cropY + cropHeight) <= img.rows);
	assert(cropHeight > 0);*/

	roi = RoiType(tbCropX, tbCropY, tbCropWidth, tbCropHeight);
}

#pragma once

#include "stdafx.h"

// c
#include <ctime> // clock_t

// cv::
#include <opencv2/core/core.hpp> // Point2d, Mat, Scalar

class FramePos {
public:
	typedef cv::Point2d Pos;

	FramePos(); // Invalid position
	FramePos(const Pos& pos);
	FramePos(const FramePos& other);

	FramePos& operator=(const FramePos& other);

	void draw(cv::Mat& img) const;

	bool valid;
	Pos pos;
private:
	cv::Scalar color;

	static const int radius = 16;
	static const int thickness = 4;
};

#pragma once

#include "stdafx.h"

// c
#include <ctime> // clock_t

// cv::
#include <opencv2/core/core.hpp> // Point2d, Mat, Scalar

class FramePos {
public:
	typedef cv::Point2d Pos;
	typedef cv::Scalar Color;

	FramePos(); // Invalid position
	FramePos(const Pos& pos);
	FramePos(const FramePos& other);

	FramePos& operator=(const FramePos& other);

	void draw(cv::Mat& img) const;

	// Factual
	bool valid;
	Pos pos;

	// Cosmetic
	Color color;
	int radius;
	int thickness;
};

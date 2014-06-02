#pragma once

#include "stdafx.h"

// c
#include <ctime> // clock_t

// std::
#include <deque>

// cv::
#include <opencv2/core/core.hpp> // Scalar

#include "FramePos.h"

class BallBeatEstimator {
public:
	typedef cv::Scalar Color;

	BallBeatEstimator(const clock_t& clock, const Color& color = Color(0, 0, 255));
	void addMeasurement(const clock_t& clock, const FramePos& pos);
	void draw(cv::Mat& img) const;
private:
	typedef std::deque<FramePos> PosHistory;
	PosHistory posHistory;
	size_t historyCapacity;
	Color color;

	FramePos averagePos;
};

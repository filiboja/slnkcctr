#pragma once

#include "stdafx.h"

// c
#include <ctime> // clock_t

// cv::
#include <opencv2/core/core.hpp> // Point2i

#include "BallBeatEstimator.h"
#include "FramePos.h"

class BeatEstimator {
public:
	typedef cv::Point2i Pos;

	BeatEstimator(const clock_t& clock = clock());
	void addMeasurement(const clock_t& clock, const std::vector<FramePos>& positions);
	void draw(cv::Mat& img) const;
private:
	BallBeatEstimator estimator0;
	BallBeatEstimator estimator1;
};

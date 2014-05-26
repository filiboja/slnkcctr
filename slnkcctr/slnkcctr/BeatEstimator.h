#pragma once

#include "stdafx.h"

// c
#include <ctime> // clock_t

// cv::
#include <opencv2/core/core.hpp> // Point2i

class BeatEstimator {
public:
	typedef cv::Point2i Pos;
	bool estimate(clock_t clock, Pos pos);
private:
	clock_t lastClock;
	double ballY;
};

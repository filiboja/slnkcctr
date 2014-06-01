#pragma once

#include "stdafx.h"

// c
#include <ctime> // clock_t

// std::
#include <list>

// cv::
#include <opencv2/core/core.hpp> // Point2i

#include "FramePos.h"

class BallBeatEstimator {
public:
	typedef cv::Point2d Pos;
	BallBeatEstimator(const clock_t& clock);
	void addMeasurement(const clock_t& clock, const FramePos& pos);
	void draw(cv::Mat& img) const;
private:
	clock_t lastClock; // in clocks
	Pos ballPos; // in pixels
	cv::Vec2d ballVel; // in pixels per second
	const double forceFactor;
	Pos prevPos;
	mutable bool beat;
	clock_t prevBeatClock;

	typedef std::list<FramePos> PosHistory;
	PosHistory posHistory;
	size_t historyCapacity;
};

#include "stdafx.h"

// c
#include <cassert>
#include <cmath> // exp
#include <ctime> // clock_t, CLOCKS_PER_SEC

// std::
#include <iostream> // cout

// cv::
#include <opencv2/core/core.hpp> // circle

#include "BallBeatEstimator.h"

BallBeatEstimator::BallBeatEstimator(const clock_t& clock)
	: lastClock(clock), forceFactor(10.0), ballPos(0.0, 0.0), ballVel(100.0, 0.0),
	historyCapacity(30)
{}

void BallBeatEstimator::addMeasurement(const clock_t& clock, const FramePos& framePos) {
	posHistory.push_back(framePos);
	if (posHistory.size() > historyCapacity) {
		posHistory.pop_front();
	}

	const Pos& pos = framePos.pos;

	if (pos.x < 0 || pos.y < 0) {
		// Invalid position
		return;
	}

	// Manage time
	assert(clock >= lastClock);
	clock_t clockDiff = clock - lastClock;
	lastClock = clock;
	double timeDiff = (double)clockDiff / CLOCKS_PER_SEC; // in seconds

	cv::Vec2d posDiff = pos - prevPos;
	double posDiffNorm = cv::norm(posDiff);
	prevPos = pos;

	/*if (posDiffNorm / timeDiff > 1000.0) {
		// Too large change; discard.
		return;
	}*/

	assert(clock >= prevBeatClock);
	double maxBps = 3;
	if (clock - prevBeatClock < (double)CLOCKS_PER_SEC / maxBps) {
		// Too short time from previous beat
		return;
	}
	//std::cout << "?";
	//std::cout.flush();

	if (pos.y >= prevPos.y) {
		// Not falling
		return;
	}
	//std::cout << "!";
	//std::cout.flush();

	if (timeDiff > 1.0 / 10.0) {
		// Too long time from previous valid measurement
		return;
	}

	if (posDiffNorm / timeDiff > 100.0) {
		std::cout << ".";
		std::cout.flush();
		prevBeatClock = clock;
		beat = true;
	}

	ballPos = pos;

	/*
	// Calculate forces
	cv::Vec2d posDiff = pos - ballPos;
	double diffNorm = cv::norm(posDiff);
	cv::Vec2d diffNormalized = posDiff * (1.0 / diffNorm);
	if (diffNorm <= 0) {
		return;
	}

	double springTolerance = 50.0;
	double springFactor = 1.0;
	if (diffNorm > springTolerance) {
		springFactor = pow(0.5, diffNorm - springTolerance);
	}
	cv::Vec2d forceSpring = 1000.0 * posDiff * springFactor;

	cv::Vec2d forceAir = 100.0 * -ballVel * cv::norm(ballVel);

	cv::Vec2d force = forceSpring + forceAir;

	// Apply force
	ballVel += force * timeDiff;

	// Apply velocity
	ballPos += cv::Point2d(ballVel * timeDiff);
	*/
}

void BallBeatEstimator::draw(cv::Mat& img) const {
	const int radius = 8;
	const cv::Scalar color = cv::Scalar(0, 0, 255);
	cv::circle(img, ballPos, radius, color, 4);
	if (beat) {
		cv::Rect rec(0, 0, 640, 480);
		cv::rectangle(img, rec, color, 16);
		beat = false;
	}

	for (PosHistory::const_iterator it = posHistory.begin(); it != posHistory.end(); ++it) {
		FramePos pos = *it;
		pos.draw(img);
		//cv::line(img, pos, pos, color);
		//cv::circle(img, pos, radius, color, 4);
	}
}

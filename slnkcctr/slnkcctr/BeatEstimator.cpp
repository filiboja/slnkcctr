#include "stdafx.h"

#include <cassert>

#include "BeatEstimator.h"

BeatEstimator::BeatEstimator(const clock_t& clock)
	: estimator0(clock, BallBeatEstimator::Color(0, 0, 255), 440), estimator1(clock, BallBeatEstimator::Color(0, 255, 0), 220)
{}

void BeatEstimator::addMeasurement(const clock_t& clock, const std::vector<FramePos>& positions) {
	assert(positions.size() >= 2);
	estimator0.addMeasurement(clock, positions[0]);
	estimator1.addMeasurement(clock, positions[1]);
}

void BeatEstimator::draw(cv::Mat& img) const {
	estimator0.draw(img);
	estimator1.draw(img);
}

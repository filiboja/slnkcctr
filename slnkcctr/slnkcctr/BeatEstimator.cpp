#include "stdafx.h"

#include <cassert>

#include "BeatEstimator.h"

/*bool BeatEstimator::estimate(clock_t clock, Pos pos) {
	bool result = false;
	double y = pos.y;
	if (y >= ballY + 50) {
		// Beat!
		result = true;
		ballY = y;
	}
	ballY = (ballY * 0.7) + (y * 0.3);
	return result;
}*/

BeatEstimator::BeatEstimator(const clock_t& clock)
	: estimator0(clock), estimator1(clock)
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

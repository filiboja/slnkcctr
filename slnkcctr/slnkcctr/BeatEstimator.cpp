#include "stdafx.h"

#include "BeatEstimator.h"

bool BeatEstimator::estimate(clock_t clock, Pos pos) {
	bool result = false;
	double y = pos.y;
	if (y >= ballY + 50) {
		// Beat!
		result = true;
		ballY = y;
	}
	ballY = (ballY * 0.7) + (y * 0.3);
	return result;
}

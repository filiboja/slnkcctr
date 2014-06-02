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

BallBeatEstimator::BallBeatEstimator(const clock_t& clock, const Color& color)
	: historyCapacity(30), color(color)
{
	averagePos.color = color;
	averagePos.radius = 8;
	averagePos.thickness = 4;
}

void BallBeatEstimator::addMeasurement(const clock_t& clock, const FramePos& framePos) {
	posHistory.push_front(framePos);
	if (posHistory.size() > historyCapacity) {
		posHistory.pop_back();
	}

	averagePos.valid = false;
	averagePos.pos = FramePos::Pos(0.0, 0.0);
	size_t count = 0;
	size_t num = 3;
	size_t id = 0;
	for (PosHistory::const_iterator it = posHistory.begin(); it != posHistory.end(); ++it) {
		const FramePos& framePos = *it;
		if (framePos.valid) {
			averagePos.pos += framePos.pos;
			++count;
		}
		++id;
		if (id >= num) {
			break;
		}
	}
	if (count >= 2) {
		averagePos.pos *= 1.0 / count;
		averagePos.valid = true;
	}
}

void BallBeatEstimator::draw(cv::Mat& img) const {
	/*if (beat) {
		cv::Rect rec(0, 0, 640, 480);
		cv::rectangle(img, rec, color, 16);
		beat = false;
	}*/

	averagePos.draw(img);

	int thickness = 4;
	
	FramePos::Pos lastValidPos;
	bool validPos = false;
	size_t id = posHistory.size() - 1;
	for (PosHistory::const_reverse_iterator it = posHistory.rbegin(); it != posHistory.rend(); ++it) {
		assert(id >= 0);
		FramePos framePos = *it;
		if (framePos.valid) {
			FramePos::Pos thisPos = framePos.pos;
			if (validPos) {
				double weight = 1.0 - ((double)id / historyCapacity);
				assert(weight >= 0.0);
				assert(weight <= 1.0);
				Color colorCur = color * weight;
				cv::line(img, lastValidPos, thisPos, colorCur, thickness);
			} else {
				validPos = true;
			}
			lastValidPos = thisPos;
		}
		--id;
	}
}

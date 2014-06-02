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
	: historyCapacity(30), color(color), beat(false), phase(ASCENT), sinceBeat(100)
{
	averagePos.color = color;
	averagePos.radius = 8;
	averagePos.thickness = 4;
}

void BallBeatEstimator::addMeasurement(const clock_t& clock, const FramePos& framePos) {
	soundPlayer.update();
	posHistory.push_front(framePos);
	if (posHistory.size() > historyCapacity) {
		posHistory.pop_back();
	}

	averagePos.valid = false;
	averagePos.pos = FramePos::Pos(0.0, 0.0);
	size_t count = 0;
	size_t num = 4;
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

	switch (phase) {
	case ASCENT:
		if (averagePos.valid && posHistory.size() >= 2 && posHistory[0].valid && posHistory[1].valid) {
			double yDiff = posHistory[0].pos.y - averagePos.pos.y;
			if (yDiff >= 32.0) {
				// Current position is far below the average position
				phase = DESCENT;
			}
		}
		break;
	case DESCENT:
		if (!averagePos.valid) {
			// Slinky end has disappeared (obstructed or left the camera)
			beat = true;
			phase = ASCENT;
		} else {
			if (posHistory.size() >= 1 && posHistory[0].valid) {
				if (posHistory.size() >= 2 && posHistory[2].valid) {
					double yDiff = posHistory[0].pos.y - posHistory[1].pos.y;
					if (yDiff <= 8.0 && yDiff >= -16.0) {
						beat = true;
					}
				}
				assert(averagePos.valid);
				double yDiff = posHistory[0].pos.y - averagePos.pos.y;
				if (yDiff <= 4.0) {
					// Current position is not far below the average position
					beat = true;
					phase = ASCENT;
				}
			}
		}
		break;
	}
	if (sinceBeat < 6) {
		beat = false;
	}
	if (beat) {
		sinceBeat = 0;
	} else {
		sinceBeat++;
	}
}

void BallBeatEstimator::draw(cv::Mat& img) const {
	if (beat) {
		cv::Rect rec(0, 0, img.size().width, img.size().height);
		cv::rectangle(img, rec, color, 16);
		beat = false;
		soundPlayer.play();
	}

	switch (phase) {
	case ASCENT:
		color[0] = 0;
		break;
	case DESCENT:
		color[0] = 255;
		break;
	}

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

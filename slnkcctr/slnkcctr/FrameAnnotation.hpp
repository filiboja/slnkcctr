#pragma once

#include "stdafx.h"

// std::
#include <ostream> // ostream

// cv::
#include <opencv2/core/core.hpp> // Point2i

struct FrameAnnotation {
public:
	typedef cv::Point2i Pos;
	Pos slinky0;
	Pos slinky1;
	Pos handLeft;
	Pos handRight;
};

inline std::ostream& operator<<(std::ostream& os, const FrameAnnotation& obj) {
	// TODO: Print obj into os here.
	return os;
}

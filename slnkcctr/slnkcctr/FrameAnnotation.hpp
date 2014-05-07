#pragma once

#include "stdafx.h"

// std::
#include <ostream> // ostream, endl

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
	os << "slinky0: " << obj.slinky0 << std::endl;
	os << "slinky1: " << obj.slinky1 << std::endl;
	os << "handLeft: " << obj.handLeft << std::endl;
	os << "handRight: " << obj.handRight << std::endl;
	return os;
}

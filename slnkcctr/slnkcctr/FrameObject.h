#pragma once

#include "stdafx.h"

// cv::
#include <opencv2/core/core.hpp> // Point2i, Mat, circle

class FrameObject {
public:
	typedef cv::Point2i Pos;
	typedef cv::Scalar Color;

	FrameObject(const char * const& id, const Pos pos, const int radius, const Color color);

	void draw(cv::Mat& img) const;
	std::ostream& print(std::ostream& os) const; // inline

	friend bool operator<(const FrameObject& left, const FrameObject& right);
private:
	const char * const id; // slinky0, slinky1, handLeft, handRight
	const Pos pos;
	const int radius;
	const Color color;
};

std::ostream& operator<<(std::ostream& os, const FrameObject& obj);

// Only id and pos are taken into account.
// If id and pos match, left and right are considered equal.
bool operator<(const FrameObject& left, const FrameObject& right);

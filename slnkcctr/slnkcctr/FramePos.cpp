#include "stdafx.h"

#include "FramePos.h"

FramePos::FramePos()
	: valid(false), pos(0.0, 0.0), color(0, 0, 255), radius(16), thickness(4)
{}

FramePos::FramePos(const Pos& pos)
	: valid(true), pos(pos), color(0, 0, 255), radius(16), thickness(4)
{}

FramePos::FramePos(const FramePos& other)
	: valid(other.valid), pos(other.pos), color(other.color), radius(other.radius), thickness(other.thickness)
{}

FramePos& FramePos::operator=(const FramePos& other)
{
	valid = other.valid;
	pos = other.pos;
	color = other.color;
	radius = other.radius;
	thickness = other.thickness;
	return *this;
}

void FramePos::draw(cv::Mat& img) const
{
	if (valid) {
		circle(img, pos, radius, color, thickness);
	}
}

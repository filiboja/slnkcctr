#include "stdafx.h"

#include "FramePos.h"

FramePos::FramePos()
	: valid(false), color(0, 0, 255)
{}

FramePos::FramePos(const Pos& pos)
	: valid(true), pos(pos), color(0, 0, 255)
{}

FramePos::FramePos(const FramePos& other)
	: valid(other.valid), pos(other.pos), color(other.color)
{}

FramePos& FramePos::operator=(const FramePos& other)
{
	valid = other.valid;
	pos = other.pos;
	color = other.color;
	return *this;
}

void FramePos::draw(cv::Mat& img) const
{
	if (valid) {
		circle(img, pos, radius, color, thickness);
	}
}

#include "stdafx.h"

#include "FrameObject.h"

#include <cassert> // assert
#include <cstring> // strcmp

FrameObject::FrameObject(const std::string& id, const bool valid, const Pos pos, const int radius, const Color color)
	: id(id), valid(valid), pos(pos), radius(radius), color(color)
{}

void
FrameObject::draw(cv::Mat& img) const {
	if (valid) {
		circle(img, pos, radius, color, 4);
	}
}

std::ostream&
FrameObject::print(std::ostream& os) const {
	os << id << ": ";
	if (valid) {
		os << pos;
	} else {
		os << "invalid position";
	}
	return os;
}
}

std::ostream&
operator<<(std::ostream& os, const FrameObject& obj) {
	return obj.print(os);
}

bool
operator<(const FrameObject& left, const FrameObject& right) {
	// id
	if (left.id != right.id) {
		return left.id < right.id;
	}

	// pos
	if (left.pos != right.pos) {
		if (left.pos.x != right.pos.x) {
			return left.pos.x < right.pos.x;
		} else {
			assert(left.pos.y != right.pos.y);
			return left.pos.y < right.pos.y;
		}
	}
	
	return false;
}

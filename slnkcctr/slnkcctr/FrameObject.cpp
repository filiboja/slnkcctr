#include "stdafx.h"

#include "FrameObject.h"

#include <cassert> // assert
#include <cstring> // strcmp

FrameObject::FrameObject(const char * const& id, const Pos pos, const int radius, const Color color)
	: id(id), pos(pos), radius(radius), color(color)
{}

void
FrameObject::draw(cv::Mat& img) const {
	circle(img, pos, radius, color);
}

std::ostream&
FrameObject::print(std::ostream& os) const {
	return os << id << ": " << pos;
}

std::ostream&
operator<<(std::ostream& os, const FrameObject& obj) {
	return obj.print(os);
}

bool
operator<(const FrameObject& left, const FrameObject& right) {
	// id
	int idDiff = strcmp(left.id, right.id);
	if (idDiff != 0) {
		return idDiff < 0;
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

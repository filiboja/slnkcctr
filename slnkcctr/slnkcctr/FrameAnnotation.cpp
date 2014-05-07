#include "stdafx.h"

#include "FrameAnnotation.hpp"

void
FrameAnnotation::insert(const FrameObject& object) {
	objects.insert(object);
}

void
FrameAnnotation::draw(cv::Mat& img) const {
	for (Objects::iterator it = objects.begin(); it != objects.end(); ++it) {
		it->draw(img);
	}
	return;
}

std::ostream&
FrameAnnotation::print(std::ostream& os) const {
	for (Objects::iterator it = objects.begin(); it != objects.end(); ++it) {
		os << *it << std::endl;
	}
	return os;
}

std::ostream&
operator<<(std::ostream& os, const FrameAnnotation& obj) {
	return obj.print(os);
}

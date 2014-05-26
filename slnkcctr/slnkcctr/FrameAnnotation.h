#pragma once

#include "stdafx.h"

// std::
#include <ostream> // ostream, endl
#include <set> // set

#include "FrameObject.h" // FrameObject

class FrameAnnotation {
public:
	void insert(const FrameObject& object);
	void draw(cv::Mat& img) const;
	std::ostream& print(std::ostream& os) const; // block
	const FrameObject& get() const;
private:
	typedef std::set<FrameObject> Objects;
	Objects objects;
};

std::ostream& operator<<(std::ostream& os, const FrameAnnotation& obj);

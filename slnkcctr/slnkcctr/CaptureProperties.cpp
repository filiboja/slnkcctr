#include "stdafx.h"

#include "CaptureProperties.h"

CaptureProperties::CaptureProperties(std::ostream& os)
: os(os) {
	init();
}

void
CaptureProperties::init() {
	properties.insert(Property(CV_CAP_PROP_FRAME_WIDTH, "Frame width"));
	properties.insert(Property(CV_CAP_PROP_FRAME_HEIGHT, "Frame height"));
	properties.insert(Property(CV_CAP_PROP_FPS, "FPS"));
	properties.insert(Property(CV_CAP_PROP_MODE, "Mode"));
	properties.insert(Property(CV_CAP_PROP_FORMAT, "Format"));
}

void
CaptureProperties::print(cv::VideoCapture& cap) const {
	for (Properties::iterator it=properties.begin(); it!=properties.end(); ++it) {
		const PropertyBase& index = it->first;
		const char * const& description = it->second;
		os << description << ": " << cap.get(index) << std::endl;
	}
}

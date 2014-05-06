// Prints `VideoCapture` properties

#include "stdafx.h"

// std::
#include <iostream> // cout, endl, ostream
#include <set> // set
#include <utility> // pair

// cv::
#include <opencv2/highgui/highgui.hpp> // VideoCapture

class CaptureProperties {
public:
	CaptureProperties(std::ostream& os = std::cout);
	void print(cv::VideoCapture& cap) const;
private:
	void init();

	// http://en.cppreference.com/w/cpp/language/enum
	typedef int PropertyBase;

	typedef std::pair<const PropertyBase, const char * const> Property; // <index, description>
	typedef std::set<Property> Properties;

	Properties properties;
	std::ostream& os;
};

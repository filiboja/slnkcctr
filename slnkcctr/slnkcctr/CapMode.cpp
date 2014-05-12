#include "stdafx.h"

#include "CapMode.h"

// std::
#include <string> // string

// http://stackoverflow.com/a/5212594
std::istream& operator>>(std::istream& in, CapMode& capMode)
{
	std::string token;
	in >> token;
	switch (token[0]) {
	case 'd':
		capMode = CAP_MODE_DEVICE;
		break;
	case 'f':
		capMode = CAP_MODE_FILE;
		break;
	//default:
		//throw po::validation_error(po::validation_error::invalid_option_value);
	}
	return in;
}

std::ostream& operator<<(std::ostream& out, const CapMode& capMode)
{
	switch (capMode) {
	case CAP_MODE_DEVICE:
		out << "device";
		break;
	case CAP_MODE_FILE:
		out << "file";
		break;
	}
	return out;
}

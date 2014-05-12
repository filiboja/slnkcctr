#pragma once

#include "stdafx.h"

// std::
#include <iostream> // istream, ostream

enum CapMode { CAP_MODE_DEVICE = 0, CAP_MODE_FILE = 1 };

std::istream& operator>>(std::istream& in, CapMode& capMode);
std::ostream& operator<<(std::ostream& out, const CapMode& capMode);

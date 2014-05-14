#pragma once

#include "stdafx.h"

// boost::
#include <boost/program_options.hpp>
namespace po = boost::program_options;

// std::
#include <iostream> // cerr, endl
#include <string> // string

void loadConfigFile(const std::string& filename, const po::options_description& options, po::variables_map& vm,
	std::ostream& cerr = std::cerr);

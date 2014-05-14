#include "stdafx.h"

#include "program_options.h"

void loadConfigFile(const std::string& filename, const po::options_description& options, po::variables_map& vm,
	std::ostream& cerr)
{
	try {
		po::basic_parsed_options<char> fileOptions = po::parse_config_file<char>(filename.c_str(), options);
		po::store(fileOptions, vm);
	} catch (po::reading_file& e) {
		cerr << e.what() << std::endl;
	}
}

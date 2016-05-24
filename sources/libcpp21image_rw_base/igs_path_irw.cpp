#include "igs_path_irw.h"

const std::string igs::path::ext_from_path(const std::string&file_path)
{
	std::string::size_type index = file_path.rfind('.');
	if (index == std::string::npos) { return ""; }
	return file_path.substr(++index);
}


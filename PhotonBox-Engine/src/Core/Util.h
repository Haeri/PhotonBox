#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

namespace Util
{
	void split(const std::string& line, const std::string& delimiter, std::vector<std::string>& output);
}

#endif // UTIL_H
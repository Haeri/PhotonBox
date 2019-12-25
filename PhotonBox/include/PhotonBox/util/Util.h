#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

namespace Util
{
	void split(const std::string& line, const std::string& delimiter, std::vector<std::string>& output);
	unsigned char* compress(unsigned char* data, size_t ucp_size, size_t* cp_size_out);
	unsigned char* decompress(unsigned char* data, size_t cp_size, size_t ucp_size);
	/***
	* first argument is the argument count excluding this argument
	* every event argument is the data cast to an unsigned char pointer
	* every odd argument is the size of the data
	***/
	unsigned char* combineBuffer(int argc, ...);

	unsigned char* loadIcon(const std::string& fileName, int& width, int& height);
	void freeIcon(unsigned char* data);
}

#endif // UTIL_H

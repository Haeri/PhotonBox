#include "PhotonBox/core/Util.h"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

namespace Util
{
	void split(const std::string& line, const std::string& delimiter, std::vector<std::string>& output)
	{
		std::string s = line;
		size_t pos = 0;
		std::string token;

		while ((pos = s.find(delimiter)) != std::string::npos)
		{
			token = s.substr(0, pos);
			output.push_back(token);
			s.erase(0, pos + delimiter.length());
		}
		output.push_back(s);
	}
}

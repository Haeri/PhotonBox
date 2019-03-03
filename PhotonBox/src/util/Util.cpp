#include "PhotonBox/util/Util.h"

#include <stdarg.h>

#include "zlib/zlib.h"

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

	unsigned char* compress(unsigned char* data, size_t ucp_size, size_t* cp_size_out)
	{
		unsigned char* data_out = (unsigned char*)malloc(ucp_size);

		z_stream defstream;
		defstream.zalloc = Z_NULL;
		defstream.zfree = Z_NULL;
		defstream.opaque = Z_NULL;

		defstream.avail_in = (uInt)ucp_size;
		defstream.next_in = (Bytef *)data;
		defstream.avail_out = (uInt)ucp_size;
		defstream.next_out = (Bytef *)data_out;

		deflateInit(&defstream, Z_BEST_COMPRESSION);
		deflate(&defstream, Z_FINISH);
		deflateEnd(&defstream);
		*cp_size_out = defstream.total_out;

		return data_out;
	}


	unsigned char* decompress(unsigned char* data, size_t cp_size, size_t ucp_size)
	{
		unsigned char* data_out = (unsigned char*)malloc(ucp_size);

		z_stream infstream;
		infstream.zalloc = Z_NULL;
		infstream.zfree = Z_NULL;
		infstream.opaque = Z_NULL;

		infstream.avail_in = (uInt)cp_size;
		infstream.next_in = (Bytef *)data;
		infstream.avail_out = (uInt)ucp_size;
		infstream.next_out = (Bytef *)data_out;

		inflateInit(&infstream);
		inflate(&infstream, Z_NO_FLUSH);
		inflateEnd(&infstream);

		return data_out;
	}

	unsigned char * combineBuffer(int argc, ...)
	{
		unsigned char* buff;
		std::vector<std::pair<unsigned char*, int>> buffList;
		int buffSize = 0;
		va_list args;
		va_start(args, argc);
		for (size_t i = 0; i < argc / 2.0; i++)
		{
			unsigned char* data = va_arg(args, unsigned char*);
			size_t size = va_arg(args, size_t);
			buffList.push_back(std::make_pair(data, size));
			buffSize += buffList[i].second;
		}
		va_end(args);

		int offset = 0;
		buff = (unsigned char*)malloc(buffSize);
		
		for (int i = 0; i < argc / 2.0; i++)
		{
			unsigned char* curr = buff + offset;
			memcpy(curr, buffList[i].first, buffList[i].second);
			offset += buffList[i].second;
		}
		
		return buff;
	}
}

#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config
{
public:
	struct Profile
	{
		int width;
		int height;
		bool fullscreen;
		bool vsync;
		bool supersampling;
		bool showFPS;
		bool fpsProfiling;
	};

	static Profile profile;

	const std::string FILE_PATH = "config.cfg";
	void readConfig();
	void createDefault();
};

#endif // CONFIG_H

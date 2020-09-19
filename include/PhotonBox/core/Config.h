#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config
{
public:
	struct Profile
	{
		std::string appName = "PhotonBox";
		int width			= 1280;
		int height			= 720;
		bool fullscreen		= false;
		bool vsync			= false;
		float supersampling	= 1.0f;
		bool showFPS		= false;
		bool fpsProfiling	= false;
	};

	static Profile profile;

	const std::string FILE_PATH = "config.cfg";
	void readConfig();
	void createDefault();
};

#endif // CONFIG_H

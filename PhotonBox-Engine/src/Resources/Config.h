#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config {
public:
	struct Profile {
		int width;
		int height;
		bool fullscreen;
		bool vsync;
		bool supersampling;
	};

	static Profile profile;

	const std::string FILE_PATH = "config.cfg";
	void readConfig();
};

#endif // CONFIG_H
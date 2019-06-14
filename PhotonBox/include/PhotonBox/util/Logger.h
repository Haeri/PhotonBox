#ifndef LOGGER_H
#define LOGGER_H

#ifdef PB_PLATFORM_WIN
#include <windows.h>
#endif

class Logger
{
public:
	enum Type
	{
#ifdef PB_PLATFORM_WIN
		DEF = 15,
		INFO = 9,
		WARN = 6,
		ERR = 4,
		CONFIRM = 10
#else
		DEF = 0,
		INFO = 34,
		WARN = 33,
		ERR = 31,
		CONFIRM = 32
#endif
	};
	static void logn(std::string message, Type type)
	{
		log(message + "\n\r", type);
	}

	static void log(std::string message, Type type)
	{
#ifdef PB_PLATFORM_WIN
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, type);
		std::cout << message;
		SetConsoleTextAttribute(hConsole, DEF);
#else
		std::cout << "\x1B[" << std::to_string(type) << "m" << message << "\033[0m";
#endif
	}
};

#endif // LOGGER_H


//HANDLE Logger::hConsole;
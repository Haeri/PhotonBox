#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <sstream> 

#ifdef PB_PLATFORM_WIN
#include <windows.h>
#undef max
#undef min
#undef near
#undef far
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

	struct LogLevel
	{
		std::string name;
	};

	struct LevelCore : public LogLevel { std::string name = "Core"; };
	struct LevelApplication : public LogLevel { std::string name = "Application"; };

	template <typename ... args >
	static void log(args ... to_print)
	{
		_printStart(DEF, to_print...);
	}
	template <typename ... args >
	static void logln(args ... to_print)
	{
		_printStart(DEF, to_print...);
		std::cout << "\n";
	}

	template <typename ... args >
	static void info(args ... to_print)
	{
		_printStart(INFO, to_print...);
	}
	template <typename ... args >
	static void infoln(args ... to_print)
	{
		_printStart(INFO, to_print...);
		std::cout << "\n";
	}

	template <typename ... args >
	static void confirm(args ... to_print)
	{
		_printStart(CONFIRM, to_print...);
	}
	template <typename ... args >
	static void confirmln(args ... to_print)
	{
		_printStart(CONFIRM, to_print...);
		std::cout << "\n";
	}

	template <typename ... args >
	static void warn(args ... to_print)
	{
		_printStart(WARN, to_print...);
	}
	template <typename ... args >
	static void warnln(args ... to_print)
	{
		_printStart(WARN, to_print...);
		std::cout << "\n";
	}

	template <typename ... args >
	static void err(args ... to_print)
	{
		_printStart(ERR, to_print...);
	}
	template <typename ... args >
	static void errln(args ... to_print)
	{
		_printStart(ERR, to_print...);
		std::cerr << "\n";
	}

	static void hr()
	{
		std::cout << "---------------------------------------------------\n";
	}

	static void br()
	{
		std::cout << "\n";
	}



private:
	// Start
	template <typename ... args >
	static void _printStart(Type type, args ... to_print)
	{
		std::string time_str;

#ifdef PB_LOG_TO_FILE
		std::time_t time = std::time(nullptr);
		time_str = std::ctime(&time);
		time_str.pop_back();

		time_str = "[" + time_str + "] ";
#endif

#ifdef PB_PLATFORM_WIN
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, type);
		if (type == ERR)
			std::cerr << time_str << "ERROR: ";
		else
			std::cout << time_str;

#else
		if (type == ERR)
			std::cerr << "\x1B[" << std::to_string(type) << "m" << time_str << "ERROR: ";
		else
			std::cout << "\x1B[" << std::to_string(type) << "m" << time_str;
#endif

		_print(type, to_print...);
	}


	// Mid
	template <typename T, typename ... args >
	static void _print(Type type, T current, args... next)
	{
		if (type == ERR)
			std::cerr << current << " ";
		else
			std::cout << current << " ";

		_print(type, next...);
	}


	// End
	template <typename T >
	static void _print(Type type, T last)
	{
#ifdef PB_PLATFORM_WIN
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		if (type == ERR)
			std::cerr << last;
		else
			std::cout << last;

		SetConsoleTextAttribute(hConsole, DEF);
#else
		if (type == ERR)
			std::cerr << last << "\033[0m";
		else
			std::cout << last << "\033[0m";
#endif
	}
};

#endif // LOGGER_H

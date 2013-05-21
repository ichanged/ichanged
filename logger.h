#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>

class logger {
public:
	logger();
	virtual ~logger();

	static void fatal(const char *format, ...);
	static void error(const char *format, ...);
	static void warn(const char *format, ...);
private:
	static void log(int level, std::string info);

	static const int LOG_FATAL = 5;
	static const int LOG_ERROR = 5;
	static const int LOG_WARN = 5;
	static const int LOG_INFO = 5;
	static const int LOG_DEBUG = 5;
};

#endif

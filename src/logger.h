#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>
#include <string.h>
#include <errno.h>

#define ERRSTR		(strerror(errno))

class logger {
public:
	logger();
	virtual ~logger();

	static void fatal(const char *format, ...);
	static void error(const char *format, ...);
	static void warn(const char *format, ...);
	static void info(const char *format, ...);
	static void debug(const char *format, ...);

private:
	static void log(int level, std::string info);

	static const int LOG_FATAL = 5;
	static const int LOG_ERROR = 4;
	static const int LOG_WARN = 3;
	static const int LOG_INFO = 2;
	static const int LOG_DEBUG = 1;
};

#endif

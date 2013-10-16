#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "logger.h"

std::ofstream logger::fout;
const char *logger::LOG_FILE = "ichanged.log";

logger::logger()
{
}

logger::~logger()
{
}

void
logger::init()
{
	logger::fout.open(LOG_FILE);
	logger::info("[%s %d] log module init completely", 
				__FILE__, __LINE__);
}

void
logger::destroy()
{
	logger::fout.close();	
	logger::info("[%s %d] log module end completely", __FILE__, __LINE__);
}

void
logger::fatal(const char *format, ...)
{
	va_list ap;
	char buf[1024];

	va_start(ap, format);
	vsprintf(buf, format, ap);
	va_end(ap);

	logger::log(logger::LOG_FATAL, buf);
	exit(EXIT_FAILURE);
}

void
logger::error(const char *format, ...)
{
	va_list ap;
	char buf[1024];

	va_start(ap, format);
	vsprintf(buf, format, ap);
	va_end(ap);

	logger::log(logger::LOG_ERROR, buf);
	exit(EXIT_FAILURE);
}

void
logger::warn(const char *format, ...)
{
	va_list ap;
	char buf[1024];

	va_start(ap, format);
	vsprintf(buf, format, ap);
	va_end(ap);

	logger::log(logger::LOG_WARN, buf);
}

void
logger::info(const char *format, ...)
{
	va_list ap;
	char buf[1024];

	va_start(ap, format);
	vsprintf(buf, format, ap);
	va_end(ap);

	logger::log(logger::LOG_INFO, buf);
}

void
logger::debug(const char *format, ...)
{
	va_list ap;
	char buf[1024];

	va_start(ap, format);
	vsprintf(buf, format, ap);
	va_end(ap);

	logger::log(logger::LOG_DEBUG, buf);
}

void
logger::log(int level, std::string info)
{
//	FILE *fp;
	const char *slevel = NULL;

//	if(level > logger::LOG_WARN) {
//		fp = stderr;
//	} else {
//		fp = stdout;
//	}

	switch(level) {
	case logger::LOG_FATAL:
		slevel = "FATAL";
		break;
	case logger::LOG_ERROR:
		slevel = "ERROR";
		break;
	case logger::LOG_WARN:
		slevel = "WARN";
		break;
	case logger::LOG_INFO:
		slevel = "INFO";
		break;
	case logger::LOG_DEBUG:
		slevel = "DEBUG";
		break;
	default:
		slevel = "UNKNOW";
		break;
	}

//	fprintf(fp, "[%s] %s\n", slevel, info.c_str());
	logger::fout << "[" << slevel << "] " << info.c_str() << std::endl;
	if(level > logger::LOG_WARN) {
		exit(EXIT_FAILURE);
	}
}

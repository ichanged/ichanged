#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "logger.h"

logger::logger()
{
}

logger::~logger()
{
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
logger::log(int level, std::string info)
{
	if(level > logger::LOG_WARN) {
		fprintf(stderr, "%s\n", info.c_str());
		if(level > logger::LOG_WARN) {
			exit(EXIT_FAILURE);
		}
	} else {
		fprintf(stdout, "%s\n", info.c_str());
	}
}

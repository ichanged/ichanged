#include <stdio.h>
#include <stdarg.h>
#include <string>

#include "error.h"

Error::Error(std::string _file, int _line, const char *format, ...) throw()
:file(_file), line(_line)
{
	va_list ap;
	char tmp[1024] = {0};

	va_start(ap, format);
	vsnprintf(tmp, sizeof(tmp), format, ap);
	va_end(ap);

	this->error_msg.assign(tmp);
}

std::string 
Error::what()
{
	return this->error_msg;	
}

std::string
Error::get_file()
{
	return this->file; 
}

int
Error::get_line()
{
	return this->line;
}


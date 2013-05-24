#include <string.h>

#include "file.h"

file::file()
{
}

file::file(std::string filename, const struct stat *s)
{
	this->filename = filename;
	memcpy(&this->s, s, sizeof(struct stat));
}

file::~file()
{
}

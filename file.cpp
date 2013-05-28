#include <string>

#include "file.h"

file::file()
{
}

file::file(const struct stat *s, bool new_create, std::string filename)
:node(s, new_create)
{
	this->filename = filename;
}

file::~file()
{
}

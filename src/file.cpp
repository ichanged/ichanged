#include <string.h>
#include <string>

#include "file.h"

file::file()
{
}

file::file(const struct stat *s, bool new_create, std::string filename)
:node(s, new_create)
{
	this->_filename = filename;
}

file::~file()
{
}

void
file::modify(const struct stat *ns)
{
	memcpy(&this->_ns, ns, sizeof(struct stat));
	this->_modify = true;
	this->_change = true;
}

void
file::attrib(const struct stat *ns)
{
	memcpy(&this->_ns, ns, sizeof(struct stat));
	this->_attrib = true;
	this->_change = true;
}

void 
file::idelete(const struct stat *ns)
{
	this->_ns.st_size = -2 ;
	this->_delete = true;
	this->_change = true;
}

std::string
file::get_filename()
{
	return this->_filename;
}

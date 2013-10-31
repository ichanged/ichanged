#include <string.h>
#include <string>

#include "file.h"

file::file()
{
}

file::file(const struct stat *s, bool new_create, std::string filename, 
		bool link = false)
:node(s, new_create, link)
{
	this->_filename = filename;
}

file::~file()
{
}

void
file::modify(const struct stat *ns)
{
	this->set_time();
	memcpy(&this->_ns, ns, sizeof(struct stat));
	this->_modify = true;
	this->_change = true;
}

void
file::attrib(const struct stat *ns)
{
	this->set_time();
	memcpy(&this->_ns, ns, sizeof(struct stat));
	this->_attrib = true;
	this->_change = true;
}

void 
file::idelete(const struct stat *ns)
{
	this->set_time();
	this->_ns.st_size = -2 ;
	this->_delete = true;
	this->_change = true;
}

bool
file::write(const struct stat *ns)
{
	if (this->_ns.st_size < ns->st_size) {
		this->set_time();
		memcpy(&this->_ns, ns, sizeof(struct stat));
		this->_modify = true;
		this->_change = true;
		return true;
	}
	return false;
}

std::string
file::get_filename()
{
	return this->_filename;
}

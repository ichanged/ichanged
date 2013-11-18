#include <string.h>
#include <string>

#include "file.h"
#include "datum.h"

file::file()
{
}

file::file(const struct stat *s, bool new_create, std::string filename,
		bool read, bool link = false)
:node(s, new_create, read, link)
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

void
file::export_file(std::string dir)
{
	int length;
	std::string path;	
	char *name = NULL;
	unsigned char len[2] = {0};

	path = dir + "/" + this->_filename;
	name = (char *)path.c_str();
	length = strlen(name) + 1;
	memcpy(len, &length, sizeof(len));

	fwrite(len, sizeof(len), 1, datum::fp);
	fwrite(name, strlen(name) + 1, 1, datum::fp);
	fwrite(&this->_base, sizeof(this->_base), 1, datum::fp);
}

#include <sys/stat.h>

#include "event.h"

void
event::set_path(const std::string path)
{
	this->_path = path;
}

std::string
event::get_path()
{
	return this->_path;
}

void
event::set_type(const int type)
{
	this->_type = type;
}

int
event::get_type()
{
	return this->_type;
}

void
event::set_base_size(off_t size)
{
	this->_base_size = size;
}

off_t
event::get_base_size()
{
	return this->_base_size;
}

void
event::set_current_size(off_t size)
{
	this->_current_size = size;
}

off_t
event::get_current_size()
{
	return this->_current_size;
}

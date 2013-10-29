#include <sys/stat.h>

#include "event.h"

event::event()
:
_path(""),
_type(0),
_base_size(0),
_current_size(0)
{
}

event::~event()
{
}

void
event::set_path(const std::string path)
{
	this->_path = path;
}

std::string
event::get_path()
{
	if (this->_type & event::TYPE_DIRECTORY) {
		return this->_path + "/";
	} else {
		return this->_path;
	}
}

void
event::set_chg_time(const char *time)
{
	strncpy(this->_chg_time, time, strlen(time));
}	

char *
event::get_chg_time()
{
	return this->_chg_time;
}

void
event::set_type(const int type)
{
	this->_type = type;
}

void
event::add_type(const int type)
{
	this->_type |= type;
}

int
event::get_type()
{
	return this->_type;
}

std::string
event::get_type_string()
{
	std::string type = std::string("");
	if (this->_type & event::TYPE_ATTRIB) {
		type += "A";
	}
	if (this->_type & event::TYPE_MODIFY) {
		type += "M";
	}
	if (this->_type & event::TYPE_CREATE) {
		type += "+";
	}
	if (this->_type & event::TYPE_DELETE) {
		type += "D";
	}
	return type;
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

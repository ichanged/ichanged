#include <string.h>
#include <sys/stat.h>

#include "node.h"

node::node()
{
}

node::node(const struct stat *stat_tmp, bool new_create)
:
_modify(false),
_attrib(false),
_delete(false),
_change(false)
{
	this->new_create = new_create;
	if (new_create) {
		this->_base.st_size = -2;		
		memcpy(&this->_ns, stat_tmp, sizeof(struct stat));
	} else {
		memcpy(&this->_base, stat_tmp, sizeof(struct stat));
		memcpy(&this->_ns, stat_tmp, sizeof(struct stat));
	}
}

node::~node()
{
}

char *
node::get_time()
{
	return this->_chg_time;
}

void
node::set_time()
{
	char tmp[1000] = {0};
	time_t now;		
	struct tm date;

	now = time(NULL);
	localtime_r(&now, &date);

	sprintf(tmp, "%d-%02d-%02d %02d:%02d:%02d", date.tm_year + 1900,
			date.tm_mon + 1, date.tm_mday, date.tm_hour, 
			date.tm_min, date.tm_sec);	
	strncpy(this->_chg_time, tmp, 20);
}

bool
node::is_new_create()
{
	return this->new_create;
}

bool
node::is_modify()
{
	return this->_modify;
}

bool
node::is_attrib()
{
	return this->_attrib;
}

bool
node::is_change()
{
	return this->_change;
}

bool
node::is_delete()
{
	return this->_delete;
}

off_t
node::get_base_size()
{
	return this->_base.st_size;
}

off_t
node::get_current_size()
{
	return this->_ns.st_size;
}

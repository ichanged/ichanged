#include <string.h>
#include <sys/stat.h>

#include "node.h"

node::node()
{
}

node::node(const struct stat *base, bool new_create)
:
_modify(false),
_attrib(false),
_delete(false),
_change(false)
{
	this->_new_create = new_create;
	memcpy(&this->_base, base, sizeof(struct stat));
	if (this->_new_create) {
		memcpy(&this->_ns, base, sizeof(struct stat));
	}
}

node::~node()
{
}

bool
node::is_new_create()
{
	return this->_new_create;
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

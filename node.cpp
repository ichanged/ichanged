#include <string.h>
#include <sys/stat.h>

#include "node.h"

node::node()
{
}

node::node(const struct stat *base, bool new_create)
:
_modify(false),
_attrib(false)
{
	this->_new_create = new_create;
	memcpy(&this->_base, base, sizeof(struct stat));
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

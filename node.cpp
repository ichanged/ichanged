#include <string.h>
#include <sys/stat.h>

#include "node.h"

node::node()
{
}

node::node(const struct stat *s, bool new_create)
{
	memcpy(&this->s, s, sizeof(struct stat));
	this->new_create = new_create;
}

node::~node()
{
}

void
node::modify(const struct stat *ns)
{
	memcpy(&this->ns, ns, sizeof(struct stat));
	this->modified = true;
}

bool
node::is_new_create()
{
	return this->new_create;
}

bool
node::is_modified()
{
	return this->modified;
}

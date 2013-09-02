#ifndef _NODE_H
#define _NODE_H

#include <sys/stat.h>
#include <vector>

#include "event.h"

class node {
public:
	node();
	node(const struct stat *base, bool new_create);
	~node();

	bool is_new_create();
	bool is_modify();
	bool is_attrib();

	bool is_change();

	off_t get_base_size();
	off_t get_current_size();
protected:
	bool _new_create;
	bool _modify;
	bool _attrib;
	bool _change;

	struct stat _base, _ns;
};

#endif
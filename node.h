#ifndef _NODE_H
#define _NODE_H

#include <sys/stat.h>

class node {
public:
	node();
	node(const struct stat *base, bool new_create);
	~node();

	bool is_new_create();
	bool is_modify();
	bool is_attrib();
protected:
	bool _new_create;
	bool _modify;
	bool _attrib;

	struct stat _base, _ns;
};

#endif

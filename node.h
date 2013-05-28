#ifndef _NODE_H
#define _NODE_H

#include <sys/stat.h>

class node {
public:
	node();
	node(const struct stat *s, bool new_create);
	~node();

	void modify(const struct stat *ns);

	bool is_new_create();
	bool is_modified();
protected:
	bool new_create;
	bool modified;
	struct stat s, ns;
};

#endif

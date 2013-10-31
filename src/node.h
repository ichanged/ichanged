#ifndef _NODE_H
#define _NODE_H

#include <sys/stat.h>
#include <stdio.h>
#include <vector>

#include "event.h"

class node {
public:
	node();
	node(const struct stat *base, bool new_create, bool link);
	~node();

	off_t get_base_size();
	off_t get_current_size();

	char *get_time();
	void set_time();
	void set_link_path(std::string link_path);
	std::string get_link_path();

	bool is_new_create();
	bool is_modify();
	bool is_attrib();
	bool is_delete();
	bool is_link();

	bool is_change();

	bool new_create;
protected:
	bool _modify;
	bool _attrib;
	bool _delete;
	bool _change;
	bool _link;

	char _chg_time[25];
	std::string _link_path;
	struct stat _base, _ns;
};

#endif

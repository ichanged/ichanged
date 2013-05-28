#ifndef _WATCHER_H_
#define _WATCHER_H_

#include <sys/stat.h>
#include <string>
#include <map>

#include "watch.h"

class watcher {
public:
	watcher();
	virtual ~watcher();

	void add_watch(int wd, const struct stat *s, bool new_create,
		std::string path);
	watch get_watch(int wd);
	void remove_watch(int wd);

	void add_file(const struct stat *s, bool new_create, std::string path);

	void file_create(int wd, std::string name);
	void file_attrib(int wd, std::string name);

	void print_diff_result();
private:
	std::map<int, watch> watch_map;
};

extern watcher *g_watcher;

#endif

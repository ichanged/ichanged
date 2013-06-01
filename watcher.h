#ifndef _WATCHER_H_
#define _WATCHER_H_

#include <sys/stat.h>
#include <string>
#include <map>
#include <set>

#include "watch.h"

class watcher {
public:
	watcher();
	virtual ~watcher();

	void init_watch(int wd, const struct stat *s, std::string path);
	void add_watch(int wd, const struct stat *s, std::string path);
	watch get_watch(int wd);
	void remove_watch(int wd);

	void dir_attrib(int wd, std::string name);

	void init_file(const struct stat *s, std::string path);
	void add_file(const struct stat *s, std::string path);
	void file_create(int wd, std::string name);
	void file_attrib(int wd, std::string name);

	void print_diff_result();
private:
	std::map<int, watch> _watch_map;
	std::set<int> _watch_set;
};

extern watcher *g_watcher;

#endif

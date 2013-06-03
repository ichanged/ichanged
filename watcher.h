#ifndef _WATCHER_H_
#define _WATCHER_H_

#include <sys/stat.h>
#include <string>
#include <map>
#include <set>

#include "watch.h"

class watcher {
public:
	static void init_watch(int wd, const struct stat *s, std::string path);
	static void add_watch(int wd, const struct stat *s, std::string path);
	static watch get_watch(int wd);
	static void remove_watch(int wd);

	static void dir_attrib(int wd, std::string name);

	static void init_file(const struct stat *s, std::string path);
	static void add_file(const struct stat *s, std::string path);
	static void file_create(int wd, std::string name);
	static void file_attrib(int wd, std::string name);

	static void print_diff_result();
private:
	static std::map<int, watch> _watch_map;
	static std::set<int> _watch_set;
};

#endif

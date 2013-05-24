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

	void add_watch(int wd, std::string path, const struct stat *s);
	watch get_watch(int wd);

	void add_file(std::string path, const struct stat *s);
private:
	std::map<int, watch> watch_map;
};

extern watcher *g_watcher;

#endif

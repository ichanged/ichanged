#ifndef _MONITOR_H_
#define _MONITOR_H_

#include <string>

typedef int (*ftw_func)(const char *fpath, const struct stat *sb, int typeflag);

class monitor {
public:
	monitor(std::string dir);
	virtual ~monitor();
private:
	void add_watch(std::string dir);
	static int do_add_watch(const char *fpath, const struct stat *sb,
		int typeflag);

	std::string dir;
	static int inotify_fd;
	static int mask;
};

extern monitor *g_monitor;

#endif

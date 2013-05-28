#ifndef _MONITOR_H_
#define _MONITOR_H_

#include <string>
#include <sys/inotify.h>
#include <limits.h>

typedef int (*ftw_func)(const char *fpath, const struct stat *sb, int typeflag);

class monitor {
public:
	monitor(std::string dir);
	virtual ~monitor();

	void start();
	void init_monitor(std::string dir);
	void add_monitor(std::string dir);
	void remove_monitor(int wd);
private:
	static int do_init_monitor(const char *fpath, const struct stat *sb,
		int typeflag);
	static int do_add_monitor(const char *fpath, const struct stat *sb,
		int typeflag);
	
	std::string dir;
	static int inotify_fd;
	static int mask;

	static const int event_max_size =
		sizeof(struct inotify_event) + NAME_MAX + 1;
};

extern monitor *g_monitor;

#endif

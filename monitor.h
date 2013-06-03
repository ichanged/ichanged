#ifndef _MONITOR_H_
#define _MONITOR_H_

#include <string>
#include <sys/inotify.h>
#include <limits.h>
#include <pthread.h>

typedef int (*ftw_func)(const char *fpath, const struct stat *sb, int typeflag);

class monitor {
public:
	static void init();
	static void wait();
	static void destory();

	static void *start(void *arg);
	static void init_monitor(std::string dir);
	static void add_monitor(std::string dir);
	static void remove_monitor(int wd);
private:
	static int do_init_monitor(const char *fpath, const struct stat *sb,
		int typeflag);
	static int do_add_monitor(const char *fpath, const struct stat *sb,
		int typeflag);
	
	static std::string dir;
	static int inotify_fd;
	static int mask;
	static pthread_t thread_id;

	static const int event_max_size =
		sizeof(struct inotify_event) + NAME_MAX + 1;
};

extern monitor *g_monitor;

#endif

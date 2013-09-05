#ifndef _MONITOR_H_
#define _MONITOR_H_

#include <string>
#include <sys/inotify.h>
#include <limits.h>
#include <pthread.h>

/*
 * 监视事件模块
 */

typedef int (*ftw_func)(const char *fpath, const struct stat *sb, int typeflag);

class monitor {
public:
	static void init();
	static void wait();
	static void destroy();

	static void *start(void *arg);
	static void init_monitor(std::string dir);
	static void add_monitor(std::string dir);
	static void remove_monitor(int wd);
private:
	static int do_init_monitor(const char *fpath, const struct stat *sb,
		int typeflag);
	static int do_add_monitor(const char *fpath, const struct stat *sb,
		int typeflag);

	/**
	 * 判断路径是否为隐藏文件夹或者隐藏文件
	 * @param fpath 文件路径
	 * @return 返回true代表是隐藏文件夹或文件，false代表不是
	 */
	static bool is_path_hidden(const char *fpath);
	
	static std::string dir;
	static int inotify_fd;
	static int mask;
	static pthread_t thread_id;

	static const int event_max_size =
		sizeof(struct inotify_event) + NAME_MAX + 1;
};

#endif

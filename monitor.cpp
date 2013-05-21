#include <sys/inotify.h>
#include <unistd.h>
#include <ftw.h>
#include <iostream>

#include "monitor.h"
#include "logger.h"

monitor *g_monitor = NULL;

monitor::monitor(std::string dir)
{
	monitor::inotify_fd = inotify_init();
	if(monitor::inotify_fd == -1) {
		logger::fatal("create inotify file descriptor error");
	}
	monitor::mask = IN_CREATE;

	this->dir = dir;
	this->add_watch(dir);
}

monitor::~monitor()
{
	close(monitor::inotify_fd);
}

void
monitor::add_watch(std::string dir)
{
	int ret;

	ret = ftw(dir.c_str(), (ftw_func)monitor::do_add_watch, 500);
	if(ret == -1) {
		logger::fatal("traverse directory %s error", dir.c_str());
	}
}

int
monitor::do_add_watch(const char *fpath, const struct stat *sb,
		int typeflag)
{
	int wd;

	if(typeflag != FTW_D) {
		return 0;
	}
	wd = inotify_add_watch(monitor::inotify_fd, fpath, monitor::mask);
	if(-1 == wd) {
		logger::warn("add watch to '%s' error", fpath);
	}
	return 0;
}

int monitor::inotify_fd = 0;
int monitor::mask = 0;

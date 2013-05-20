#include <sys/inotify.h>
#include <unistd.h>
#include <ftw.h>
#include <iostream>

#include "monitor.h"
#include "logger.h"

monitor::monitor(std::string dir)
{
	monitor::inotify_fd = inotify_init();
	if(monitor::inotify_fd == -1) {
		logger::fatal("init inotify error");
	}
	this->add_watch(dir);
}

monitor::~monitor()
{
	close(monitor::inotify_fd);
}

void
monitor::add_watch(std::string dir)
{
	ftw(dir.c_str(), (ftw_func)monitor::do_add_watch, 500);
}

int
monitor::do_add_watch(const char *fpath, const struct stat *sb,
		int typeflag)
{
	if(typeflag != FTW_D) {
		return 0;
	}
	std::cout<<fpath<<std::endl;
	return 0;
}

int monitor::inotify_fd = 0;

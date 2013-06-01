#include <sys/inotify.h>
#include <unistd.h>
#include <ftw.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "monitor.h"
#include "watcher.h"
#include "handler.h"
#include "logger.h"
#include "watch.h"

monitor *g_monitor = NULL;

monitor::monitor(std::string dir)
{
	monitor::inotify_fd = inotify_init();
	if(monitor::inotify_fd == -1) {
		logger::fatal("create inotify file descriptor error");
	}
	monitor::mask = IN_CREATE | IN_ATTRIB | IN_MODIFY;

	this->dir = dir;
	this->init_monitor(dir);
}

monitor::~monitor()
{
	close(monitor::inotify_fd);
}

void
monitor::start()
{
	int n;
	char buf[monitor::event_max_size] = {0};

	while(true) {
		size_t size;
		struct inotify_event *pe = NULL, *e = NULL;

		/* 读取inotify_event结构体 */
		n = read(monitor::inotify_fd, buf, monitor::event_max_size);
		if(n == -1) {
			logger::fatal("read event from inotify error: %s", ERRSTR);
		}
		pe = (struct inotify_event *)buf;
		size = sizeof(struct inotify_event) + pe->len;
		e = (struct inotify_event *)malloc(size);
		if(e == NULL) {
			logger::fatal("failed to allocate memory for inotify_event: %s",
				ERRSTR);
		}

		/* 将读取的事件拷贝到分配的内存中 */
		memcpy(e, buf, size);
		/* 事件处理模块对事件进行处理 */
		g_handler->handle_event(e);	
		/* 释放事件内存 */
		free(e);
	}
}

void
monitor::init_monitor(std::string dir)
{
	int ret;

	ret = ftw(dir.c_str(), (ftw_func)monitor::do_init_monitor, 500);
	if(ret == -1) {
		logger::fatal("traverse directory '%s' to init monitor error",
			dir.c_str());
	}
}

void
monitor::add_monitor(std::string dir)
{
	int ret;

	ret = ftw(dir.c_str(), (ftw_func)monitor::do_add_monitor, 500);
	if(ret == -1) {
		logger::fatal("traverse directory '%s' to add monitor error",
			dir.c_str());
	}
}

void
monitor::remove_monitor(int wd)
{
	g_watcher->remove_watch(wd);
}

int
monitor::do_init_monitor(const char *fpath, const struct stat *sb,
	int typeflag)
{
	int wd;

	switch(typeflag) {
	case FTW_F:
		g_watcher->init_file(sb, fpath);
		break;
	case FTW_D:
		wd = inotify_add_watch(monitor::inotify_fd, fpath, monitor::mask);
		if(-1 == wd) {
			logger::warn("add watch to '%s' error", fpath);
		}
		g_watcher->init_watch(wd, sb, fpath);
		break;
	default:
		break;
	}

	return 0;
}

int
monitor::do_add_monitor(const char *fpath, const struct stat *sb,
	int typeflag)
{
	int wd;

	switch(typeflag) {
	case FTW_F:
		g_watcher->add_file(sb, fpath);
		break;
	case FTW_D:
		wd = inotify_add_watch(monitor::inotify_fd, fpath, monitor::mask);
		if(-1 == wd) {
			logger::warn("add watch to '%s' error", fpath);
		}
		g_watcher->add_watch(wd, sb, fpath);
		break;
	default:
		break;
	}

	return 0;
}


int monitor::inotify_fd = 0;
int monitor::mask = 0;

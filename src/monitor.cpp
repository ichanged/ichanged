#include <sys/inotify.h>
#include <unistd.h>
#include <ftw.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "monitor.h"
#include "error.h"
#include "watcher.h"
#include "handler.h"
#include "logger.h"
#include "watch.h"
#include "options.h"
#include "ichanged.h"
#include "window.h"

std::string monitor::dir;
int monitor::inotify_fd = 0;
int monitor::mask = 0;
pthread_t monitor::thread_id = 0;

void
monitor::init()
{
	int status;

	std::cout << "starting monitor, please wait..." << std::endl;
	monitor::inotify_fd = inotify_init();
	if(monitor::inotify_fd == -1) {
		throw Error(__FILE__, __LINE__, 
				"create inotify file descriptor error");
	}
	//monitor::mask = IN_CREATE | IN_ATTRIB | IN_MODIFY | IN_MOVE | 
		//IN_DELETE;
	monitor::mask = IN_ALL_EVENTS;

	monitor::dir = options::directory;
	monitor::init_monitor(options::directory);

	status = pthread_create(&monitor::thread_id, NULL, monitor::start, NULL);
	if(status != 0) {
		throw Error(__FILE__, __LINE__, 
				"create monitor thread error: %s", ERRSTR);
	}
	tid[0] = monitor::thread_id; 

	logger::info("[%s %d] thread: %lu monitor module init completely", 
			__FILE__, __LINE__, monitor::thread_id);
}

void
monitor::wait()
{
	int status;
	void *ret;

	try {
		status = pthread_join(monitor::thread_id, &ret);
		if(status != 0) {
			logger::fatal("join monitor thread error: %s", ERRSTR);
		}
	} catch (...){
		endwin();	
	}
}

void
monitor::destroy()
{
	close(monitor::inotify_fd);
	logger::info("[%s %d] monitor module end completely", __FILE__,
			__LINE__);
}

void *
monitor::start(void *arg)
{
	int n;
	char buf[monitor::event_max_size] = {0};

	while(true) {
		size_t size;
		struct inotify_event *pe = NULL, *e = NULL;

		/* 读取inotify_event结构体 */
		n = read(monitor::inotify_fd, buf, monitor::event_max_size);
		if(n == -1) {
			logger::fatal("read event from inotify error: %s", 
					__FILE__, __LINE__, ERRSTR);
		}
		pe = (struct inotify_event *)buf;
		size = sizeof(struct inotify_event) + pe->len;
		e = (struct inotify_event *)malloc(size);
		if(e == NULL) {
			logger::fatal("failed to allocate memory for" 
					" inotify_event: %s", ERRSTR);
		}

		/* 将读取的事件拷贝到分配的内存中 */
		memcpy(e, buf, size);
		/* 事件处理模块对事件进行处理 */
		handler::handle_event(e);	
		/* 释放事件内存 */
		free(e);
	}
	return NULL;
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
	if (inotify_rm_watch(monitor::inotify_fd, wd) <= 0) {
		logger::warn("[%s %d]rm inotify watch failure", __FILE__, 
				__LINE__);
	}
}

int
monitor::do_init_monitor(const char *fpath, const struct stat *sb,
	int typeflag)
{
	int wd;
	std::vector<std::string>::iterator iter;
	std::string sfpath = std::string(fpath);

	// 如果配置了不监控隐藏文件与目录，则直接返回
	if (!options::watch_hidden && monitor::is_path_hidden(fpath)) {
		return 0;
	}

	// 如果当前目录是配置了不监控的特定目录，则直接返回
	for (iter = options::exclude.begin(); iter != options::exclude.end();
		++iter) {
		if (sfpath.compare(0, iter->length(), *iter) == 0) {
			return 0;
		}
	}

	switch (typeflag) {
	case FTW_F: {
		watcher::init_file(sb, fpath);
		break;
	}
	case FTW_D: {
		if (watcher::is_watch_exist(std::string(fpath)) == false) {
			wd = inotify_add_watch(monitor::inotify_fd, fpath, 
					monitor::mask);
			if (wd == -1) {
				logger::warn("add watch to '%s' error: %s", fpath, ERRSTR);
			}
			watcher::init_watch(wd, sb, std::string(fpath));
		}
		break;
	}
	default: {
		break;
	}
	}

	return 0;
}

int
monitor::do_add_monitor(const char *fpath, const struct stat *sb,
	int typeflag)
{
	int wd;
	std::vector<std::string>::iterator iter;
	std::string sfpath = std::string(fpath);

	// 如果配置了不监控隐藏文件与目录，则直接返回
	if(!options::watch_hidden && monitor::is_path_hidden(fpath)) {
		return 0;
	}
	for (iter = options::exclude.begin(); iter != options::exclude.end();
		++iter) {
		if (sfpath.compare(0, iter->length(), *iter) == 0) {
			return 0;
		}
	}

	switch(typeflag) {
	case FTW_F:
		watcher::add_file(sb, fpath);
		break;
	case FTW_D:
		wd = inotify_add_watch(monitor::inotify_fd, fpath, monitor::mask);
		if(-1 == wd) {
			logger::warn("add watch to '%s' error", fpath);
		}
		watcher::add_watch(wd, sb, fpath);
		break;
	default:
		break;
	}

	return 0;
}

/**
 * 判断路径是否为隐藏文件夹或者隐藏文件
 * @param fpath 文件路径
 * @return 返回true代表是隐藏文件夹或文件，false代表不是
 */
bool
monitor::is_path_hidden(const char *fpath)
{
	const char *slash = fpath;

	while(slash != NULL) {
		slash = strchr(slash, '/');
		if(slash != NULL) {
			if(slash[1] == '.') {
				return true;
			} else {
				slash++;
			}
		}
	}
	return false;
}

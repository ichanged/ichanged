#include <sys/stat.h>
#include <pthread.h>
#include <libgen.h>
#include <string.h>
#include <unistd.h>

#include "watcher.h"
#include "logger.h"
#include "monitor.h"
#include "record.h"

std::map<int, watch> watcher::_watch_map;
std::map<std::string, int> watcher::_wd_map;
std::set<int> watcher::_watch_set;

std::vector<event> watcher::_event_vec;

pthread_mutex_t watcher::mutex = PTHREAD_MUTEX_INITIALIZER;

void
watcher::init_watch(int wd, const struct stat *sb, std::string path)
{
	watcher::_watch_map[wd] = watch(sb, false, path);
	watcher::_wd_map[path] = wd;
}

void
watcher::add_watch(int wd, const struct stat *sb, std::string path)
{
	watch *w = NULL;

	watcher::_watch_map[wd] = watch(sb, true, path);
	w = &watcher::_watch_map[wd];
	watcher::_watch_map[wd].set_time();
	watcher::_wd_map[path] = wd;
	watcher::_watch_set.insert(wd);
	record::event_to_file(event::TYPE_CREATE, w->get_base_size(),
		w->get_current_size(), w->get_path());
}

watch
watcher::get_watch(int wd)
{
	return watcher::_watch_map[wd];
}

void
watcher::dir_delete(int wd, char *path)
{
	//TODO
	std::string path_tmp;

	path_tmp = watcher::_watch_map[wd].get_path() + "/" + std::string(path); 
	wd = watcher::_wd_map[path_tmp];
	if (watcher::_watch_map[wd].idelete()) {
		watcher::_watch_set.insert(wd);	
	}	

	monitor::remove_monitor(wd);
}

void
watcher::dir_modify(int wd, char *path)
{
	std::string path_tmp;

	path_tmp = watcher::_watch_map[wd].get_path() + "/" + std::string(path); 
	wd = watcher::_wd_map[path_tmp];
	if (watcher::_watch_map[wd].modify()) {
		watcher::_watch_set.insert(wd);
	}
}

void
watcher::dir_attrib(int wd, char *path)
{
	std::string path_tmp;

	path_tmp = watcher::_watch_map[wd].get_path() + "/" + std::string(path); 
	wd = watcher::_wd_map[path_tmp];
	if (watcher::_watch_map[wd].attrib()) {
		watcher::_watch_set.insert(wd);
	}
}

void
watcher::init_file(const struct stat *sb, std::string path)
{	
	int wd;
	watch *w;
	bool is_link = false;
	bool is_dir = true;
	struct stat sb_link;
	char buf[1024] = {0};
	std::string link_path = "";
	std::map<int, watch>::iterator pos;

	char *dir = NULL;
	char *filename = NULL;

	char *dbuf = new char[path.length() + 1];
	char *fbuf = new char[path.length() + 1];

	// 获取当前文件路径的目录名和文件名
	strcpy(dbuf, path.c_str());
	dir = dirname(dbuf);
	strcpy(fbuf, path.c_str());
	filename = basename(fbuf);

	// 判断是否是链接文件，如果是，获取其源文件路径
	if (lstat(path.c_str(), &sb_link) == -1) {
		logger::warn("[%s %d]lstat error: %s", __FILE__, __LINE__, 
				ERRSTR);		
	}
	if (S_ISLNK(sb_link.st_mode)) {
		is_link = true;	
	}
	if (S_ISDIR(sb_link.st_mode)) {
		is_dir = true;		
	}
	if (is_link) {
		if (readlink(path.c_str(), buf, sizeof(buf)) == -1) {
			logger::fatal("[%s %d]readlink error: %s", __FILE__, __LINE__,
					ERRSTR);
		}
	}
	link_path.assign(buf);
	// 如果链接文件在当前目录
	if (link_path[0] != '/') {
		link_path = (std::string)dir + "/" + link_path;	
	}

	// 查找此文件目录是否处于监控中，如果不包含，则添加监控
	for(pos = watcher::_watch_map.begin(); pos != watcher::_watch_map.end();
		++pos) {
		w = &pos->second;
		if(w->get_path() == dir) {
			break;
		}
	}
	if (pos == watcher::_watch_map.end()) {
		wd = inotify_add_watch(monitor::inotify_fd, dir, 
				monitor::mask);
		watcher::_watch_map[wd] = watch(sb, false, dir, false, true);
		watcher::_wd_map[dir] = wd;
		w = &watcher::_watch_map[wd];
	}
	w->file_init(sb, filename, is_link, link_path);

	if (is_link) {
		if (stat(link_path.c_str(), &sb_link) == -1) {

			logger::warn("[%s %d]stat error: %s", __FILE__, 
					__LINE__, ERRSTR);
		}
		return init_file(&sb_link, link_path); 
	} else {
		return;
	}
}

void
watcher::add_file(const struct stat *sb, std::string path)
{
	int wd;
	watch *w;
	bool link = false;
	struct stat sb_link;
	char buf[1024] = {0};
	std::string link_path = "";
	std::map<int, watch>::iterator pos;

	char *dir = NULL;
	char *filename = NULL;

	char *dbuf = new char[path.length() + 1];
	char *fbuf = new char[path.length() + 1];

	strcpy(dbuf, path.c_str());
	dir = dirname(dbuf);

	strcpy(fbuf, path.c_str());
	filename = basename(fbuf);

	// 判断是否是链接文件，如果是，获取其源文件路径
	if (lstat(path.c_str(), &sb_link) == -1) {
		logger::fatal("[%s %d]lstat error: %s", __FILE__, __LINE__, 
				ERRSTR);		
	}
	if (S_ISLNK(sb_link.st_mode)) {
		link = true;	
	}
	if (link) {
		if (readlink(path.c_str(), buf, sizeof(buf)) == -1) {
			logger::fatal("[%s %d]readlink error: %s", __FILE__, 
					__LINE__, ERRSTR);
		}
	}
	link_path.assign(buf);
	// 如果链接文件在当前目录
	if (link_path.find("/") == std::string::npos) {
		link_path = (std::string)dir + "/" + link_path;	
	}

	// 查找此文件目录是否处于监控中，如果不包含，则添加监控
	for(pos = watcher::_watch_map.begin(); pos != watcher::_watch_map.end();
		++pos) {
		w = &pos->second;
		if(w->get_path() == dir) {
			wd = watcher::_wd_map[dir];
			watcher::_watch_set.insert(wd);
			break;
		}
	}
	if (pos == watcher::_watch_map.end()) {
		wd = inotify_add_watch(monitor::inotify_fd, dir, 
				monitor::mask);
		watcher::_watch_map[wd] = watch(sb, true, dir, false, true);
		watcher::_wd_map[dir] = wd;
		watcher::_watch_set.insert(wd);
		w = &watcher::_watch_map[wd];
		
	}

	w->file_create(filename, link, link_path);
	record::change_flag = true;

	if (link) {
		if (stat(link_path.c_str(), &sb_link) == -1) {
			logger::fatal("[%s %d]stat error: %s", __FILE__, 
					__LINE__, ERRSTR);
		}
		return watcher::add_file(&sb_link, link_path); 
	} else {
		return;
	}

}

void
watcher::file_create(int wd, std::string name)
{	
	watch *w;
	struct stat s;
	std::string path;
	std::string link_path;

	if (!watcher::_watch_map[wd].is_linked()) {
		w = &watcher::_watch_map[wd];	
		path = w->get_path() + "/" + name; 
		if (stat(path.c_str(), &s) == -1) {
			return;
		}
		return add_file(&s, path);
//		if (w->file_create(name)) {
//			watcher::_watch_set.insert(wd);
//		}
//		if (w->is_link_file(name)) {
//			link_path = w->get_file_link_path(name);
//			if (lstat(link_path.c_str(), &s) == -1) {
//				logger::fatal("[%s %d]lstat error: %s", 
//						__FILE__, __LINE__, ERRSTR);		
//			}
//			return add_file(&s, link_path);
//		} else {
//			return;
//		}
	}
}

void
watcher::file_attrib(int wd, std::string name)
{
	if (watcher::_watch_map[wd].file_attrib(name)) {
		watcher::_watch_set.insert(wd);
	}
}

void
watcher::file_modify(int wd, std::string name)
{
	if (watcher::_watch_map[wd].file_modify(name)) {
		watcher::_watch_set.insert(wd);
	}
}

void
watcher::file_delete(int wd, std::string name)
{
	watch *w;
	int wd_link;
	char *dbuf;
	char *fbuf;
	char *dir = NULL;
	char *filename = NULL;
	std::string link_path;

	// 删除链接文件
	if (watcher::_watch_map[wd].file_delete(name)) {
		watcher::_watch_set.insert(wd);
		record::change_flag = true;
	}	

	// 删除链接文件的源文件
	w = &watcher::_watch_map[wd];
	if (w->is_link_file(name)){
		link_path = w->get_file_link_path(name);
		dbuf = new char[link_path.length() + 1];
		fbuf = new char[link_path.length() + 1];
		strcpy(dbuf, link_path.c_str());
		dir = dirname(dbuf);
		strcpy(fbuf, link_path.c_str());
		filename = basename(fbuf); 

		wd_link = watcher::_wd_map[dir];
		if (wd_link == wd) {
			return;
		}
		record::change_flag = true;
		watcher::file_delete(wd_link, filename);
//		if (watcher::_watch_map[wd].file_delete(filename)) {
//			watcher::_watch_set.insert(wd);
//		}

	} else {
		return;
	}
}

void
watcher::file_write(int wd, std::string name)
{
	if (watcher::_watch_map[wd].file_write(name)) {
		watcher::_watch_set.insert(wd);	
		record::change_flag = true;
	}	
}

void
watcher::lock()
{
	int status;

	status = pthread_mutex_lock(&watcher::mutex);
	if (status != 0) {
		logger::fatal("lock watcher error: %s", ERRSTR);
	}
}

void
watcher::unlock()
{
	int status;

	status = pthread_mutex_unlock(&watcher::mutex);
	if (status != 0) {
		logger::fatal("unlock watcher error: %s", ERRSTR);
	}
}

std::vector<event> *
watcher::generate_snapshot()
{
	std::set<int>::iterator pos;

	watcher::_event_vec.clear();
	for (pos = watcher::_watch_set.begin(); pos != watcher::_watch_set.end();
		++pos) {
		watch &w = watcher::_watch_map[*pos];
		if(w.is_new_create() || w.is_change()) {
			w.generate_snapshot(&watcher::_event_vec);
		}
	}
	return &watcher::_event_vec;
}

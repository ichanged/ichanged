#include <sys/stat.h>
#include <pthread.h>
#include <libgen.h>
#include <string.h>
#include <unistd.h>

#include "watcher.h"
#include "logger.h"
#include "monitor.h"
#include "options.h"
#include "record.h"

std::map<int, watch> watcher::_watch_map;
std::map<std::string, int> watcher::_wd_map;
std::set<int> watcher::_watch_set;

std::vector<event> watcher::_event_vec;

pthread_mutex_t watcher::mutex = PTHREAD_MUTEX_INITIALIZER;

bool
watcher::is_watch_exist(std::string path)
{
	if (watcher::_wd_map.find(path) != watcher::_wd_map.end()) {
		return true;
	}
	return false;
}

int
watcher::init_watch(const struct stat *sb, std::string path, bool linked)
{
	int wd;

	if (watcher::is_watch_exist(path)) {
		return watcher::_wd_map[path];
	}

	wd = inotify_add_watch(monitor::inotify_fd, path.c_str(), monitor::mask);
	if (wd == -1) {
		logger::warn("add watch to '%s' error: %s", path.c_str(), 
				ERRSTR);
	}

	watcher::_watch_map[wd] = watch(sb, false, path, false, false, linked);
	watcher::_wd_map[path] = wd;

	return wd;
}

int
watcher::add_watch(const struct stat *sb, std::string path, bool link, 
		bool linked)
{
	int wd;
	watch *w = NULL;

	if (watcher::is_watch_exist(path)) {
		wd = watcher::_wd_map[path];
		if (!watcher::_watch_map[wd].is_delete()) {
			return wd;	
		}
	}

	wd = inotify_add_watch(monitor::inotify_fd, path.c_str(), 
			monitor::mask);
	if (wd == -1) {
		logger::warn("add watch to '%s' error: %s", 
				path.c_str(), ERRSTR);
	}
	
	watcher::_watch_map[wd] = watch(sb, true, path, true, link, linked);
	watcher::_wd_map[path] = wd;
	w = &watcher::_watch_map[wd];
	w->set_time();
	watcher::_watch_set.insert(wd);
	record::event_to_file(event::TYPE_CREATE, w->get_base_size(),
		w->get_current_size(), w->get_path().c_str());

	return wd;
}

watch *
watcher::get_watch(int wd)
{
	return &watcher::_watch_map[wd];
}

void
watcher::dir_delete(int wd, char *path)
{
	std::string path_tmp;

	path_tmp = watcher::_watch_map[wd].get_path() + "/" + std::string(path); 
	if (watcher::_wd_map.find(path_tmp) == watcher::_wd_map.end()) {
		return;
	}
	wd = watcher::_wd_map[path_tmp];
	if (watcher::_watch_map[wd].idelete()) {
		watcher::_watch_set.insert(wd);	
	}	

	monitor::remove_monitor(wd);
}

void
watcher::dir_delete_datum(int wd)
{
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
	struct stat sb_link;
	char buf[PATH_MAX] = {0};
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
	if (is_link) {
		if (readlink(path.c_str(), buf, sizeof(buf)) == -1) {
			logger::warn("[%s %d]readlink error: %s", __FILE__, __LINE__,
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
		struct stat sb_dir;
		if (stat(dir, &sb_dir) == -1) {
			logger::warn("[%s %d]stat error: %s", __FILE__, 
					__LINE__, ERRSTR);
		}
		wd = watcher::init_watch(&sb_dir, dir, true);
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
	std::vector<std::string>::iterator iter;

	char *dir = NULL;
	char *filename = NULL;

	char *dbuf = new char[path.length() + 1];
	char *fbuf = new char[path.length() + 1];

	strcpy(dbuf, path.c_str());
	dir = dirname(dbuf);

	strcpy(fbuf, path.c_str());
	filename = basename(fbuf);

	// 是否是隐藏文件或者不监控的文件
	if(!options::watch_hidden && monitor::is_path_hidden(path.c_str())) {
		return;
	}
	for (iter = options::exclude.begin(); iter != options::exclude.end();
		++iter) {
		if (path.compare(0, iter->length(), *iter) == 0) {
			return;
		}
	}
	
	if (S_ISDIR(sb->st_mode)) {
		monitor::add_monitor(path);	
		return;
	}

	// 判断是否是链接文件，如果是，获取其源文件路径
	if (lstat(path.c_str(), &sb_link) == -1) {
		logger::warn("[%s %d]lstat error: %s", __FILE__, __LINE__, 
				ERRSTR);		
	}
	if (S_ISLNK(sb_link.st_mode)) {
		link = true;	
	}
	if (link) {
		if (readlink(path.c_str(), buf, sizeof(buf)) == -1) {
			logger::warn("[%s %d]readlink error: %s", __FILE__, 
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
		struct stat sb_dir;
		if (stat(dir, &sb_dir) == -1) {
			logger::warn("[%s %d]stat error: %s", __FILE__, 
					__LINE__, ERRSTR);
		}
		wd = watcher::add_watch(&sb_dir, dir, false, true);
		w = &watcher::_watch_map[wd];
		
	}

	w->file_create(filename, link, link_path);

	if (link) {
		if (stat(link_path.c_str(), &sb_link) == -1) {
			logger::warn("[%s %d]stat error: %s", __FILE__, 
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
	std::string path;
	std::string link_path;
 
	path = watcher::_watch_map[wd].get_path() + "/" + name;
	if (watcher::_wd_map.find(path) != watcher::_wd_map.end()) {
		dir_delete(wd, (char *)name.c_str());
		return;
	}

	// 删除链接文件
	if (watcher::_watch_map[wd].file_delete(name)) {
		watcher::_watch_set.insert(wd);
	}	

	// 删除链接文件的源文件
	w = &watcher::_watch_map[wd];
	if (w->is_link_file(name)){
		int wd_link;
		char *dbuf;
		char *fbuf;
		char *dir = NULL;
		char *filename = NULL;

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

void
watcher::export_file()
{
	watch *w = NULL;
	std::map<int, watch>::iterator iter;	

	for(iter = watcher::_watch_map.begin(); 
			iter != watcher::_watch_map.end(); ++iter) {
		w = &iter->second;	
		w->export_file();
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

void
watcher::check_delete()
{
	int wd;
	watch *w = NULL;
	std::map<int, watch>::iterator iter;
	
	for (iter = watcher::_watch_map.begin(); iter != watcher::_watch_map.end();
			++iter) {
		wd = iter->first;
		w = &iter->second;		

		if (!w->get_read()) {
			watcher::dir_delete_datum(wd); 
		}
		w->check_datum_delete(wd);
	}
}

void
watcher::print()
{
	watch *w;
	std::map<int, watch>::iterator iter;

	for(iter = watcher::_watch_map.begin();
			iter != watcher::_watch_map.end(); ++iter) {
		w = &iter->second;
		w->print();	
	}
}

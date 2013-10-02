#include <sys/stat.h>
#include <pthread.h>
#include <libgen.h>
#include <string.h>

#include "watcher.h"
#include "logger.h"

std::map<int, watch> watcher::_watch_map;
std::set<int> watcher::_watch_set;

std::vector<event> watcher::_event_vec;

pthread_mutex_t watcher::mutex = PTHREAD_MUTEX_INITIALIZER;

void
watcher::init_watch(int wd, const struct stat *sb, std::string path)
{
	watcher::_watch_map[wd] = watch(sb, false, path);
}

void
watcher::add_watch(int wd, const struct stat *sb, std::string path)
{
	watcher::_watch_map[wd] = watch(sb, true, path);
	watcher::_watch_set.insert(wd);
}

watch
watcher::get_watch(int wd)
{
	return watcher::_watch_map[wd];
}

void
watcher::remove_watch(int wd)
{
	watch w;

	w = watcher::_watch_map[wd];
	watcher::_watch_map.erase(wd);

	//wufei
	watcher::_watch_set.insert(wd);
}

void
watcher::dir_attrib(int wd, std::string name)
{
	watcher::_watch_map[wd].attrib();
	watcher::_watch_set.insert(wd);
}

void
watcher::init_file(const struct stat *sb, std::string path)
{
	std::map<int, watch>::iterator pos;
	watch *w;

	char *dir = NULL;
	char *filename = NULL;

	char *dbuf = new char[path.length() + 1];
	char *fbuf = new char[path.length() + 1];

	strcpy(dbuf, path.c_str());
	dir = dirname(dbuf);

	strcpy(fbuf, path.c_str());
	filename = basename(fbuf);

	for(pos = watcher::_watch_map.begin(); pos != watcher::_watch_map.end();
		++pos) {
		w = &pos->second;
		if(w->get_path() == dir) {
			w->file_init(sb, filename);
			break;
		}
	}
}

void
watcher::add_file(const struct stat *sb, std::string path)
{
	std::map<int, watch>::iterator pos;
	watch *w;

	char *dir = NULL;
	char *filename = NULL;

	char *dbuf = new char[path.length() + 1];
	char *fbuf = new char[path.length() + 1];

	strcpy(dbuf, path.c_str());
	dir = dirname(dbuf);

	strcpy(fbuf, path.c_str());
	filename = basename(fbuf);

	for(pos = watcher::_watch_map.begin(); pos != watcher::_watch_map.end();
		++pos) {
		w = &pos->second;
		if(w->get_path() == dir) {
			w->file_create(filename);
			break;
		}
	}
}

void
watcher::file_create(int wd, std::string name)
{
	if (watcher::_watch_map[wd].file_create(name)) {
		watcher::_watch_set.insert(wd);
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
	if (watcher::_watch_map[wd].file_delete(name)) {
		watcher::_watch_set.insert(wd);
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

std::vector<event> *
watcher::generate_snapshot()
{
	std::set<int>::iterator pos;

	watcher::_event_vec.clear();
	for (pos = watcher::_watch_set.begin(); pos != watcher::_watch_set.end();
		++pos) {
		watch &w = watcher::_watch_map[*pos];
		//why
		if(w.is_new_create() || w.file_change()) {
			w.generate_snapshot(&watcher::_event_vec);
		}
	}
	return &watcher::_event_vec;
}

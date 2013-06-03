#include <sys/stat.h>
#include <pthread.h>
#include <libgen.h>
#include <string.h>

#include "watcher.h"
#include "logger.h"

std::map<int, watch> watcher::_watch_map;
std::set<int> watcher::_watch_set;
pthread_mutex_t watcher::mutex = PTHREAD_MUTEX_INITIALIZER;

void
watcher::init_watch(int wd, const struct stat *s, std::string path)
{
	watcher::_watch_map[wd] = watch(s, false, path);
}

void
watcher::add_watch(int wd, const struct stat *s, std::string path)
{
	watcher::_watch_map[wd] = watch(s, true, path);
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
}

void
watcher::init_file(const struct stat *s, std::string path)
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
			w->file_init(s, filename);
			break;
		}
	}
}

void
watcher::add_file(const struct stat *s, std::string path)
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
	watcher::_watch_map[wd].file_create(name);
	watcher::_watch_set.insert(wd);
}

void
watcher::file_attrib(int wd, std::string name)
{
	watcher::_watch_map[wd].file_attrib(name);
	watcher::_watch_set.insert(wd);
}

void
watcher::dir_attrib(int wd, std::string name)
{
	watcher::_watch_map[wd].attrib();
	watcher::_watch_set.insert(wd);
}

void
watcher::lock()
{
	int status;

	status = pthread_mutex_lock(&watcher::mutex);
	if(status != 0) {
		logger::fatal("lock watcher error: %s", ERRSTR);
	}
}

void
watcher::unlock()
{
	int status;

	status = pthread_mutex_unlock(&watcher::mutex);
	if(status != 0) {
		logger::fatal("unlock watcher error: %s", ERRSTR);
	}
}

void
watcher::print_diff_result()
{
	std::set<int>::iterator pos;
	watch *w;

	for(pos = watcher::_watch_set.begin(); pos != watcher::_watch_set.end();
		++pos) {
		w = &watcher::_watch_map[*pos];
		if(w->is_new_create()) {
			logger::debug("watch '%s' is new create", w->get_path().c_str());
		}
		if(w->is_file_change()) {
			logger::debug("watch '%s' file changed", w->get_path().c_str());
		}
	}
}

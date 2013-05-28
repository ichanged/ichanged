#include <sys/stat.h>
#include <libgen.h>
#include <string.h>

#include "watcher.h"
#include "logger.h"

watcher *g_watcher = NULL;

watcher::watcher()
{
}

watcher::~watcher()
{
}

void
watcher::init_watch(int wd, const struct stat *s, std::string path)
{
	this->watch_map[wd] = watch(s, false, path);
}

void
watcher::add_watch(int wd, const struct stat *s, std::string path)
{
	this->watch_map[wd] = watch(s, true, path);
}

watch
watcher::get_watch(int wd)
{
	return this->watch_map[wd];
}

void
watcher::remove_watch(int wd)
{
	watch w;

	w = this->watch_map[wd];
	this->watch_map.erase(wd);
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

	for(pos = this->watch_map.begin(); pos != this->watch_map.end(); ++pos) {
		w = &pos->second;
		if(w->get_path() == dir) {
			w->init_file(s, filename);
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

	for(pos = this->watch_map.begin(); pos != this->watch_map.end(); ++pos) {
		w = &pos->second;
		if(w->get_path() == dir) {
			w->add_file(filename);
		}
	}
}

void
watcher::file_create(int wd, std::string name)
{
	this->watch_map[wd].add_file(name);
}

void
watcher::file_attrib(int wd, std::string name)
{
	this->watch_map[wd].modify_file(name);
}

void
watcher::dir_attrib(int wd, std::string name)
{
	watch w;
	std::string path;

	w = this->watch_map[wd];
	path = w.get_path();
}

void
watcher::print_diff_result()
{
	std::map<int, watch>::iterator pos;
	watch *w;

	for(pos = this->watch_map.begin(); pos != this->watch_map.end(); ++pos) {
		w = &pos->second;
		if(w->is_new_create()) {
			logger::debug("watch '%s' is new create", w->get_path().c_str());
		}
	}
}

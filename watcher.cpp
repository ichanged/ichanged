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
watcher::add_watch(int wd, const struct stat *s, bool new_create,
	std::string path)
{
	watch w = watch(s, new_create, path);
	this->watch_map[wd] = w;
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
watcher::add_file(const struct stat *s, bool new_create, std::string path)
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
			w->add_file(s, new_create, filename);
		}
	}
}

void
watcher::file_create(int wd, std::string name)
{
	watch w;
	struct stat s;
	std::string path;

	w = this->watch_map[wd];
	path = w.get_path() + "/" + name;
	if(-1 == stat(path.c_str(), &s)) {
		logger::error("stat new file '%s' error: %s", path.c_str(), ERRSTR);
	}
	w.add_file(&s, true, name);
}

void
watcher::file_attrib(int wd, std::string name)
{
	watch w;
	struct stat s;
	std::string path;

	w = this->watch_map[wd];
	path = w.get_path() + "/" + name;
	if(-1 == stat(path.c_str(), &s)) {
		logger::error("stat file '%s' error: %s", path.c_str(), ERRSTR);
	}
	w.modify_file(&s, name);
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

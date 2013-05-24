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
watcher::add_watch(int wd, std::string path, const struct stat *s)
{
	watch w = watch(path, s);
	this->watch_map[wd] = w;
}

watch
watcher::get_watch(int wd)
{
	return this->watch_map[wd];
}

void
watcher::add_file(std::string path, const struct stat *s)
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
			w->add_file(filename, s);
		}
	}
}

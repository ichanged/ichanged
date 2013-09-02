#ifndef _WATCHER_H_
#define _WATCHER_H_

#include <sys/stat.h>
#include <pthread.h>
#include <string>
#include <map>
#include <set>
#include <vector>

#include "event.h"
#include "watch.h"

class watcher {
public:
	static void init_watch(int wd, const struct stat *sb, std::string path);
	static void add_watch(int wd, const struct stat *sb, std::string path);
	static watch get_watch(int wd);
	static void remove_watch(int wd);

	static void dir_attrib(int wd, std::string name);

	static void init_file(const struct stat *sb, std::string path);
	static void add_file(const struct stat *sb, std::string path);
	static void file_create(int wd, std::string name);
	static void file_attrib(int wd, std::string name);
	static void file_modify(int wd, std::string name);

	static std::vector<event> *generate_snapshot();

	static void lock();
	static void unlock();
private:
	static std::map<int, watch> _watch_map;
	static std::set<int> _watch_set;

	static std::vector<event> _event_vec;

	static pthread_mutex_t mutex;
};

#endif

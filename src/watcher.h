#ifndef _WATCHER_H_
#define _WATCHER_H_

#include <sys/stat.h>
#include <pthread.h>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <limits.h>

#include "event.h"
#include "watch.h"
#include "record.h"

class watcher {
public:
	static bool is_watch_exist(std::string path);

	static watch *get_watch(int wd);
	static void init_watch(int wd, const struct stat *sb, std::string path,
			bool linked = false);
	static void add_watch(int wd, const struct stat *sb, std::string path);
	static void remove_watch(int wd);

	static void dir_attrib(int wd, char *path);
	static void dir_delete(int wd, char *path);
	static void dir_delete_datum(int wd);
	static void dir_modify(int wd, char *path);

	static void init_file(const struct stat *sb, std::string path); 
	static void add_file(const struct stat *sb, std::string path);
	static void file_create(int wd, std::string name);
	static void file_attrib(int wd, std::string name);
	static void file_modify(int wd, std::string name);
	static void file_delete(int wd, std::string name);
	static void file_write(int wd, std::string name);

	static void export_file();
	static std::vector<event> *generate_snapshot();
	static void check_delete();
	static void print();

	static void lock();
	static void unlock();

	static std::map<std::string, int> _wd_map;
private:
	static std::map<int, watch> _watch_map;
	static std::set<int> _watch_set;

	static std::vector<event> _event_vec;

	static pthread_mutex_t mutex;
};

#endif

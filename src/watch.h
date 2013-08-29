#ifndef _WATCH_H_
#define _WATCH_H_

#include <string>
#include <map>
#include <set>
#include <sys/stat.h>

#include "node.h"
#include "file.h"

class watch : public node{
public:
	watch();
	watch(const struct stat *s, bool new_create, std::string path);
	~watch();

	std::string get_path();

	void attrib();

	bool file_change();
	void file_init(const struct stat *s, std::string filename);
	void file_create(std::string filename);
	void file_modify(std::string filename);
	void file_attrib(std::string filename);

	void generate_snapshot(std::vector<event> *event_vec);
private:
	std::string _get_file_path(std::string filename);
	void _get_file_stat(std::string filename, struct stat *s);
	void _get_stat(std::string path, struct stat *s);

	std::string _path;
	bool _file_change;
	std::map<std::string, file> _file_map;
	std::set<std::string> _file_set;
};

#endif

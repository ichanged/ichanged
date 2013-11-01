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
	watch(const struct stat *s, bool new_create, std::string path, 
			bool link = false);
	~watch();

	std::string get_path();

	bool attrib();
	bool idelete();
	bool modify();

	bool is_change();
	bool is_file_link(std::string filename);
	std::string get_file_link_path(std::string filename);
	int  get_file_count(); 
	void file_init(const struct stat *s, std::string filename, 
			bool link, std::string link_path = "");
	bool file_create(std::string filename);
	bool file_modify(std::string filename);
	bool file_attrib(std::string filename);
	bool file_delete(std::string filename);
	bool file_write(std::string filename);

	void generate_snapshot(std::vector<event> *event_vec);
private:
	std::string _get_file_path(std::string filename);
	bool _get_file_stat(std::string filename, struct stat *s);
	bool _get_stat(std::string path, struct stat *s);

	std::string _path;
	bool _file_change;
	bool _history_exist;
	std::map<std::string, file> _file_map;
	std::set<std::string> _file_set;
};

#endif

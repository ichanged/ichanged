#ifndef _WATCH_H_
#define _WATCH_H_

#include <map>
#include <set>
#include <string>
#include <sys/stat.h>

#include "node.h"
#include "file.h"

class watch : public node{
public:
	watch();
	watch(const struct stat *s, bool new_create, std::string path, 
			bool read, bool link = false, bool linked = false);
	~watch();

	std::string get_path();

	bool attrib();
	bool idelete();
	bool modify();

	bool is_change();
	bool is_linked();
	bool is_link_file(std::string filename);
	bool is_file_exist(std::string filename);

	std::string get_file_link_path(std::string filename);
	int  get_file_count(); 
	file *get_file(std::string filename);

	void file_init(const struct stat *s, std::string filename, 
			bool link, std::string link_path = "");
	bool file_create(std::string filename, bool link = false, 
			std::string link_path = "");
	bool file_modify(std::string filename);
	bool file_attrib(std::string filename);
	bool file_delete(std::string filename);
	bool file_write(std::string filename);

	void export_file();
	void generate_snapshot(std::vector<event> *event_vec);
	void check_datum_delete(int wd);
	void print();

private:
	std::string _get_file_path(std::string filename);
	bool _get_stat(std::string path, struct stat *s);
	bool _get_file_stat(std::string filename, struct stat *s);

	std::string _path;
	bool _file_change;
	bool _history_exist;
	bool _linked;
	std::map<std::string, file> _file_map;
	std::set<std::string> _file_set;
};

#endif

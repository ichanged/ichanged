#ifndef _WATCH_H_
#define _WATCH_H_

#include <string>
#include <map>
#include <sys/stat.h>

#include "node.h"
#include "file.h"

class watch : public node{
public:
	watch();
	watch(const struct stat *s, bool new_create, std::string path);
	~watch();

	std::string get_path();

	void init_file(const struct stat *s, std::string filename);

	void add_file(std::string filename);
	void modify_file(std::string filename);
private:
	std::string get_file_path(std::string filename);
	void get_file_stat(std::string filename, struct stat *s);

	std::string path;

	std::map<std::string, file> file_map;
};

#endif

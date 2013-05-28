#ifndef _WATCH_H_
#define _WATCH_H_

#include <string>
#include <map>

#include "node.h"
#include "file.h"

class watch : public node{
public:
	watch();
	watch(const struct stat *s, bool new_create, std::string path);
	~watch();

	std::string get_path();
	void add_file(const struct stat *s, bool new_create, std::string filename);
	void modify_file(const struct stat *s, std::string filename);
private:
	std::string path;

	std::map<std::string, file> file_map;
};

#endif

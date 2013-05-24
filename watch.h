#ifndef _WATCH_H_
#define _WATCH_H_

#include <sys/stat.h>
#include <string>
#include <map>

#include "file.h"

class watch {
public:
	watch();
	watch(std::string path, const struct stat *s);
	~watch();

	std::string get_path();
	void add_file(std::string filename, const struct stat *s);
private:
	std::string path;
	struct stat s;

	std::map<std::string, file> file_map;
};

#endif

#ifndef _FILE_H_
#define _FILE_H_

#include <sys/stat.h>
#include <string>

class file {
public:
	file();
	file(std::string filename, const struct stat *s);
	~file();
private:
	std::string filename;
	struct stat s;
};

#endif

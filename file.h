#ifndef _FILE_H_
#define _FILE_H_

#include <string>

#include "node.h"

class file : public node{
public:
	file();
	file(const struct stat *s, bool new_create, std::string filename);
	~file();
private:
	std::string filename;
};

#endif

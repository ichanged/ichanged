#ifndef _FILE_H_
#define _FILE_H_

#include <string>

#include "node.h"

class file : public node{
public:
	file();
	file(const struct stat *s, bool new_create, std::string filename);
	~file();

	void modify(const struct stat *ns);
	void attrib(const struct stat *ns);
	void idelete(const struct stat *ns);

	std::string get_filename();
private:
	std::string _filename;
};

#endif

#include "watch.h"
#include "file.h"
#include "logger.h"

watch::watch()
{}

watch::watch(const struct stat *s, bool new_create, std::string path)
:node(s, new_create)
{
	this->path = path;
}

watch::~watch()
{
}

std::string
watch::get_path()
{
	return this->path;
}

void
watch::add_file(const struct stat *s, bool new_create, std::string filename)
{
	this->file_map[filename] = file(s, new_create, filename);
}

void
watch::modify_file(const struct stat *s, std::string filename)
{
	file f = this->file_map[filename];
	f.modify(s);
}

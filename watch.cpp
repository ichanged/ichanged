#include <string.h>

#include "watch.h"
#include "file.h"
#include "logger.h"

watch::watch()
{}

watch::watch(std::string path, const struct stat *s)
{
	this->path = path;
	memcpy(&this->s, s, sizeof(struct stat));
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
watch::add_file(std::string filename, const struct stat *s)
{
	this->file_map[filename] = file(filename, s);
}

#include <sys/stat.h>

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
watch::init_file(const struct stat *s, std::string filename)
{
	this->file_map[filename] = file(s, false, filename);
}

void
watch::add_file(std::string filename)
{
	struct stat s;

	this->get_file_stat(filename, &s);
	this->file_map[filename] = file(&s, true, filename);
}

void
watch::modify_file(std::string filename)
{
	struct stat s;

	file f = this->file_map[filename];
	this->get_file_stat(filename, &s);
	f.modify(&s);
}

std::string
watch::get_file_path(std::string filename)
{
	return this->path + "/" + filename;
}

void
watch::get_file_stat(std::string filename, struct stat *s)
{
	std::string path = this->get_file_path(filename);
	if(-1 == stat(path.c_str(), s)) {
		logger::error("stat file '%s' error: %s", path.c_str(), ERRSTR);
	}
}

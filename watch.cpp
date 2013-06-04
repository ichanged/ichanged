#include <sys/stat.h>

#include "watch.h"
#include "file.h"
#include "logger.h"

watch::watch()
{}

watch::watch(const struct stat *s, bool new_create, std::string path)
:
node(s, new_create),
_path(path),
_file_change(false)
{
}

watch::~watch()
{
}

std::string
watch::get_path()
{
	return this->_path;
}

void
watch::attrib()
{
	this->_get_stat(this->_path, &this->_ns);
}

bool
watch::file_change()
{
	return this->_file_change;
}

void
watch::file_init(const struct stat *s, std::string filename)
{
	this->_file_map[filename] = file(s, false, filename);
}

void
watch::file_create(std::string filename)
{
	struct stat s;

	this->_get_file_stat(filename, &s);
	this->_file_map[filename] = file(&s, true, filename);

	this->_file_change = true;
	this->_file_set.insert(filename);
}

void
watch::file_modify(std::string filename)
{
	struct stat s;

	this->_get_file_stat(filename, &s);
	this->_file_map[filename].modify(&s);

	this->_file_change = true;
	this->_file_set.insert(filename);
}

void
watch::file_attrib(std::string filename)
{
	struct stat s;

	this->_get_file_stat(filename, &s);
	this->_file_map[filename].attrib(&s);

	this->_file_change = true;
	this->_file_set.insert(filename);
}

void
watch::generate_snapshot(std::vector<event> *event_vec)
{
	std::set<std::string>::iterator pos;

	for(pos = this->_file_set.begin(); pos != this->_file_set.end(); ++pos) {
		file &f = this->_file_map[*pos];
		if(f.is_new_create()) {
			event e;
			e.set_type(event::TYPE_CREATE);
			e.set_path(this->_get_file_path(f.get_filename()));
			e.set_base_size(f.get_base_size());
			e.set_current_size(f.get_current_size());
			event_vec->push_back(e);
		}
		if(f.is_modify()) {
			event e;
			e.set_type(event::TYPE_MODIFY);
			e.set_path(this->_get_file_path(f.get_filename()));
			e.set_base_size(f.get_base_size());
			e.set_current_size(f.get_current_size());
			event_vec->push_back(e);
		}
	}
}

std::string
watch::_get_file_path(std::string filename)
{
	return this->_path + "/" + filename;
}

void
watch::_get_file_stat(std::string filename, struct stat *s)
{
	std::string path = this->_get_file_path(filename);
	this->_get_stat(path, s);
}

void
watch::_get_stat(std::string path, struct stat *s)
{
	if(-1 == stat(path.c_str(), s)) {
		logger::error("stat file '%s' error: %s", path.c_str(), ERRSTR);
	}
}

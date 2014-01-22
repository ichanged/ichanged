#include <sys/stat.h>

#include "file.h"
#include "datum.h"
#include "watch.h"
#include "event.h"
#include "logger.h"
#include "options.h"
#include "watcher.h"

watch::watch()
:node(),
_path(""),
_file_change(false),
_history_exist(false),
_linked(false)
{
	
}

watch::watch(const struct stat *s, bool new_create, std::string path, bool read,
		bool link, bool linked)
:
node(s, new_create, read, link),
_path(path),
_file_change(false),
_history_exist(false),
_linked(linked)	
{
	
}

watch::~watch()
{
}

bool
watch::is_file_exist(std::string filename)
{
	if (this->_file_map.find(filename) != this->_file_map.end()) {
		return true;
	}	
	return false;
}

std::string
watch::get_path()
{
	return this->_path;
}

bool
watch::attrib()
{
	if (this->_get_stat(this->_path, &this->_ns)) {
		this->_attrib = true;
		this->_change = true;
		this->set_time();
		record::event_to_file(event::TYPE_DELETE, this->get_base_size(),
			this->get_current_size(), this->get_path());
		return true;
	}
	return false;
}

bool
watch::modify()
{
	if (this->_get_stat(this->_path, &this->_ns)) {
		this->_modify = true;
		this->_change = true;
		this->set_time();
		record::event_to_file(event::TYPE_DELETE, this->get_base_size(),
			this->get_current_size(), this->get_path());
		return true;
	}
	return false;
}

bool
watch::idelete()
{
	if (this->new_create) {
		this->_delete = true;
		this->_change = false;
		this->_history_exist = true;
		return false;
	}
	this->_delete = true;	
	this->_ns.st_size = -2;
	this->_change = true;
	this->set_time();
	record::event_to_file(event::TYPE_DELETE, this->get_base_size(),
		this->get_current_size(), this->get_path());
	return true;
}

bool
watch::is_change()
{
	if (this->_file_change || this->_change) {
		return true;
	}
	return false;
}

bool
watch::is_link_file(std::string filename)
{
	return this->_file_map[filename].is_link();	
}

bool
watch::is_linked()
{
	return this->_linked;
}

std::string
watch::get_file_link_path(std::string filename)
{
	return this->_file_map[filename].get_link_path();	
}

int
watch::get_file_count()
{
	return this->_file_map.size();	
}

file *
watch::get_file(std::string filename)
{
	return &this->_file_map[filename];		
}

void
watch::file_init(const struct stat *s, std::string filename, bool link, 
		std::string link_path)
{
	this->_file_map[filename] = file(s, false, filename, false, link);
	if (link) {
		this->_file_map[filename].set_link_path(link_path);
	}
}

bool
watch::file_create(std::string filename, bool link, std::string link_path)
{
	struct stat s; 
	file *f = NULL;

	if (!this->_get_file_stat(filename, &s)) {
		return false;
	}

	if (this->is_file_exist(filename)) {
		if (!this->_file_map[filename].is_delete()) {
			return false;
		}
	}

	f = &this->_file_map[filename];
	this->_file_map[filename] = file(&s, true, filename, true, link);
	this->_file_map[filename].set_time();
	this->_file_change = true;
	this->_file_set.insert(filename);
	record::event_to_file(event::TYPE_CREATE, f->get_base_size(),
			f->get_current_size(), this->_get_file_path(filename));

	if (link) {
		this->_file_map[filename].set_link_path(link_path);
	}
	return true;

}

bool
watch::file_modify(std::string filename)
{
	struct stat s;

	if (this->_file_map.find(filename) == this->_file_map.end()) {
		return false;	
	}
	if (this->_get_file_stat(filename, &s)) {
		file *f = NULL;

		f = &this->_file_map[filename];
		this->_file_map[filename].modify(&s);
		this->_file_change = true;
		this->_file_set.insert(filename);
		record::event_to_file(event::TYPE_MODIFY, f->get_base_size(),
				f->get_current_size(), this->_get_file_path(filename));
		return true;
	}
	return false;
}

bool
watch::file_attrib(std::string filename)
{
	struct stat s;

	if (this->_file_map.find(filename) == this->_file_map.end()) {
		return false;	
	}
	if (this->_get_file_stat(filename, &s)) {
		file *f = NULL;

		f = &this->_file_map[filename];
		this->_file_map[filename].attrib(&s);
		this->_file_change = true;
		this->_file_set.insert(filename);
		record::event_to_file(event::TYPE_ATTRIB, f->get_base_size(),
				f->get_current_size(), this->_get_file_path(filename));
		return true;
	}
	return false;
}

bool
watch::file_delete(std::string filename)
{
	file *f = NULL;

	if (this->_file_map.find(filename) == this->_file_map.end()) {
		return false;	
	}
	if (this->_file_map[filename].new_create) {
		this->_file_map[filename].set_delete(true);
		this->_file_set.erase(filename);
		return false;	
	}
	
	f = &this->_file_map[filename];
	this->_file_map[filename].idelete(NULL);	
	this->_file_change = true;
	this->_file_set.insert(filename);
	record::event_to_file(event::TYPE_DELETE, f->get_base_size(),
		f->get_current_size(), this->_get_file_path(filename));
	return true;
}

bool
watch::file_write(std::string filename)
{
	struct stat s;
	file *f = NULL;

//	if (!this->is_file_exist(filename)) {
//		return false;	
//	}
	if (this->_get_file_stat(filename, &s)) {
		if (this->_file_map[filename].write(&s)) {
			f = &this->_file_map[filename];
			this->_file_change = true;
			this->_file_set.insert(filename);
			record::event_to_file(event::TYPE_MODIFY, f->get_base_size(),
				f->get_current_size(), this->_get_file_path(filename));
			return true;
		}
	}	
	return false;
}

void
watch::export_file()
{
	int length;
	char flag;
	file *f = NULL;
	char *name = NULL;
	unsigned char len[2] = {0};
	std::map<std::string, file>::iterator iter;		

	// 写入目录本身的信息
	name = (char *)this->get_path().c_str();
	length = strlen(name) + 1;
	memcpy(len, &length, sizeof(len));

	// 写入路径长度
	fwrite(len, sizeof(len), 1, datum::fp);
	if (this->is_linked()) {
		flag = 1;
	} else {
		flag = 0;
	}
	// 写入是否为被链接的目录（不属于监控目录下）
	fwrite(&flag, sizeof(char), 1, datum::fp);
	// 写入路径
	fwrite(name, strlen(name) + 1, 1, datum::fp);
	// 写入文件stat
	fwrite(&this->_base, sizeof(this->_base), 1, datum::fp);
	
	// 写入目录下各个文件stat 
	for(iter = watch::_file_map.begin(); iter != watch::_file_map.end();
			++iter) {
		f = &iter->second;
		f->export_file(this->get_path());
	}
}

void
watch::generate_snapshot(std::vector<event> *event_vec)
{
	std::set<std::string>::iterator pos;

	if (!this->_history_exist && (this->is_new_create() || this->_change)) {
		event e;		
		e.add_type(event::TYPE_DIRECTORY);
		if (this->is_new_create()) {
			e.add_type(event::TYPE_CREATE);	
		}
		if (this->_modify) {
			e.add_type(event::TYPE_MODIFY);	
		}
		if (this->_attrib) {
			e.add_type(event::TYPE_ATTRIB);
		}
		if (this->_delete) {
			e.add_type(event::TYPE_DELETE);
		}
		e.set_path(this->_path);
		e.set_base_size(this->_base.st_size);
		e.set_current_size(this->_ns.st_size);
		e.set_chg_time(this->get_time());
		event_vec->push_back(e);
	}

	/* 针对监控目录下被修改过的文件生成快照 */
	for (pos = this->_file_set.begin(); pos != this->_file_set.end(); ++pos) {
		file &f = this->_file_map[*pos];
		event e;
		if(f.is_new_create()) {
			e.add_type(event::TYPE_CREATE);
		}
		if(f.is_modify()) {
			e.add_type(event::TYPE_MODIFY);
		}
		if(f.is_attrib()) {
			e.add_type(event::TYPE_ATTRIB);
		}
		if(f.is_delete()) {
			e.add_type(event::TYPE_DELETE);
		}
		if(e.get_type() == event::TYPE_NONE) {
			continue;
		}
//		/* 如果文件被修改且大小不变，不显示 */
//		if(e.get_type() == event::TYPE_MODIFY
//		&& f.get_base_size() == f.get_current_size()) {
//			continue;
//		}
		e.set_path(this->_get_file_path(f.get_filename()));
		e.set_base_size(f.get_base_size());
		e.set_current_size(f.get_current_size());
		e.set_chg_time(f.get_time());
		event_vec->push_back(e);
	}
}

std::string
watch::_get_file_path(std::string filename)
{
	return this->_path + "/" + filename;
}

bool
watch::_get_file_stat(std::string filename, struct stat *s)
{
	std::string path = this->_get_file_path(filename);
	return this->_get_stat(path, s);
}

bool
watch::_get_stat(std::string path, struct stat *s)
{
	if(-1 == stat(path.c_str(), s)) {
		return false;
	}
	return true;
}

void
watch::check_datum_delete(int wd)
{
	file *f = NULL;
	std::map<std::string, file>::iterator iter; 

	for (iter = this->_file_map.begin(); iter != this->_file_map.end();
			++iter) {
		f = &iter->second;		
		if (!f->get_read()) {
			watcher::file_delete(wd, iter->first);			
		}
	}
}

void
watch::print()
{
	std::map<std::string, file>::iterator iter;

	printf("%s\n", this->_path.c_str());
	for(iter = watch::_file_map.begin(); iter != watch::_file_map.end();
			++iter) {
		printf("%s\n", this->_get_file_path(iter->first).c_str());
	}
}

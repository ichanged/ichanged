#ifndef _EVENT_H_
#define _EVENT_H_

#include <sys/stat.h>
#include <string>

class event {
public:
	void set_path(const std::string path);
	std::string get_path();

	void set_type(const int type);
	int get_type();

	void set_base_size(off_t size);
	off_t get_base_size();

	void set_current_size(off_t size);
	off_t get_current_size();

	static const int TYPE_CREATE = 0x00000001;
	static const int TYPE_MODIFY = 0x00000002;
private:
	std::string _path;
	int _type;
	off_t _base_size;
	off_t _current_size;
};

#endif

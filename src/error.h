#ifndef _ERROR_H_
#define _ERROR_H_

#include <string>
#include <exception>

class Error {
public:
	Error(std::string file, int line, const char *format, ...) throw();
	~Error() throw();
	const char *what();
	const char *get_file();
	int get_line();

	std::string error_msg;
private:
	std::string file;
	int line;
};

#endif

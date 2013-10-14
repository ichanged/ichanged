#ifndef _ERROR_H_
#define _ERROR_H_

#include <exception>

class ichanged_error {
	
public:
	ichanged_error(const std::string& msg);
	string &what();

	std::string error_msg;
};

#endif

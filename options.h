#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <string>
#include <inttypes.h>

class options {
public:
	static void parse_args(int argc, char *argv[]);
	static std::string directory;
	static uint32_t interval;
};

#endif

#ifndef _CONFIG_H
#define _CONFIG_H

#include <iostream>
#include <string>
#include <vector>
#include <libconfig.h++>
#include <string.h>

using namespace libconfig;

class config {
public:
	static void init();
	static void write_config(Config &cfg);
	static void get_config(); 
private:
	static std::string _filename;
};

#endif

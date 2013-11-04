#ifndef _RECORD_H_
#define _RECORD_H_

#include <fstream>
#include <string>
#include <time.h>
#include <stdio.h>

class record {
public:
	static void init();
	static void get_time(); 
	static void event_to_file(char *output);	
	static void close();

	static std::ofstream file;
	static bool change_flag;
};

#endif

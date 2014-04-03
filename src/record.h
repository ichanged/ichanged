#ifndef _RECORD_H_
#define _RECORD_H_

#include <fstream>
#include <string>
#include <time.h>
#include <sstream>
#include <stdio.h>

#define	ICH_FILE_MODE		0644		
#define ICH_TYPE_LEN		50

extern void ich_record_init();

//class record {
//public:
//	static void init();
//	static void get_time(); 
//	static void get_type_string(int type);
//	static void event_to_file(int type, off_t base_size, off_t current_size,
//			std::string path);	
//	static void itoa(off_t size, std::string &size_str);
//	static void close();
//
//	static std::ofstream file;
//	static bool change_flag;
//
//};
//
#endif

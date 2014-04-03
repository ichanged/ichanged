#include "event.h"
#include "logger.h"
#include "options.h"
#include "watcher.h"
#include "record.h"

static FILE *ich_fp = NULL;

void
ich_record_init()
{
	ich_fp = fopen(options::filename.c_str(), "a");	
	if (ich_fp == NULL) {
		logger::fatal("[%s:%d] open record file error: %s", __FILE__,
				__LINE__, ERRSTR);
	}

	if (chmod(options::filename.c_str(), ICH_FILE_MODE) < 0) {
		logger::error("[%s:%d] change mode error: %s\n", __FILE__,
				__LINE__, ERRSTR);
	}
}

void
ich_get_time()
{
	time_t now;		
	struct tm date;

	now = time(NULL);
	localtime_r(&now, &date);

	fprintf(ich_fp, "[%d-%02d-%02d %02d:%02d:%02d] ", date.tm_year + 1900,
			date.tm_mon + 1, date.tm_mday, date.tm_hour, 
			date.tm_min, date.tm_sec);	
}

void
ich_get_type(int type)
{
	char type_str[ICH_TYPE_LEN] = {0}; 
	char *sptr = type_str;

	if (type & event::TYPE_ATTRIB) {
		*sptr++ = 'A';
	}
	if (type & event::TYPE_MODIFY) {
		*sptr++ = 'M';
	}
	if (type & event::TYPE_CREATE) {
		*sptr++ = '+';
	}
	if (type & event::TYPE_DELETE) {
		*sptr++ = 'D';
	}
	*sptr = '\0';

	fprintf(ich_fp, "%s  ", type_str);
}

void
ich_event_to_file(int type, off_t base_size, off_t current_size, char *path)
{
	ich_get_time();
	ich_get_type(type);

	if (base_size == -2) {
		fprintf(ich_fp, "--");	
	} else {
		fprintf(ich_fp, "%ld  ", base_size);
	}

	if (current_size == -2) {
		fprintf(ich_fp, "--");	
	} else {
		fprintf(ich_fp, "%ld ", current_size);	
	}

	fprintf(ich_fp, "\n");

}

void
ich_record_close()
{
	fclose(ich_fp);
}

//std::ofstream record::file; 
//bool record::change_flag = false;  
//
//void
//record::init()
//{
//	record::file.open(options::filename.c_str(), std::ios::app);
//}
//	
//void
//record::get_time()
//{
//	char tmp[1000] = {0};
//	time_t now;		
//	struct tm date;
//
//	now = time(NULL);
//	localtime_r(&now, &date);
//
//	sprintf(tmp, "[%d-%02d-%02d %02d:%02d:%02d] ", date.tm_year + 1900,
//			date.tm_mon + 1, date.tm_mday, date.tm_hour, 
//			date.tm_min, date.tm_sec);	
//	record::file << tmp;
//}
//
//void
//record::get_type_string(int type)
//{
//	std::string type_str = std::string("");
//	if (type & event::TYPE_ATTRIB) {
//		type_str += "A";
//	}
//	if (type & event::TYPE_MODIFY) {
//		type_str += "M";
//	}
//	if (type & event::TYPE_CREATE) {
//		type_str += "+";
//	}
//	if (type & event::TYPE_DELETE) {
//		type_str += "D";
//	}
//
//	record::file << type_str << " ";
//	
//}
//
//void
//record::event_to_file(int type, off_t base_size, off_t current_size, 
//		std::string path)
//{
//	std::string bs_size, ns_size;
//
//	record::get_time();
//	record::get_type_string(type);
//
//	record::itoa(base_size, bs_size);
//	record::itoa(current_size, ns_size);
//	record::file << base_size << "  " << ns_size << "  " << path << std::endl; 
//
//}
//
//void
//record::itoa(off_t size, std::string &size_str)
//{
//	std::stringstream ss;
//
//	if (size == -2) {
//		ss << "--";
//	} else {
//		ss << size;
//	}
//	size_str = ss.str();
//}
//
//void
//record::close()
//{
//	record::file.close();	
//}

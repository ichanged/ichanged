#include "options.h"
#include "watcher.h"
#include "event.h"
#include "record.h"

std::ofstream record::file; 
bool record::change_flag = false;  

void
record::init()
{
	record::file.open(options::filename.c_str(), std::ios::app);
}
	
void
record::get_time()
{
	char tmp[1000] = {0};
	time_t now;		
	struct tm date;

	now = time(NULL);
	localtime_r(&now, &date);

	record::file << "\n=========================================================\n";
	sprintf(tmp, "[%d-%02d-%02d %02d:%02d:%02d] ", date.tm_year + 1900,
			date.tm_mon + 1, date.tm_mday, date.tm_hour, 
			date.tm_min, date.tm_sec);	
	record::file << tmp;
}

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
//	record::file << type_str;
//	
//}

void
record::event_to_file(int type, off_t base_size, off_t current_size, 
		std::string path)
{
	//record::get_type_string(type);
}

//void
//record::_itoa(off_t size, std::string &size_str)
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

void
record::close()
{
	record::file.close();	
}

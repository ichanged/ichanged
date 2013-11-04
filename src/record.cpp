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
	sprintf(tmp, "%d-%02d-%02d %02d:%02d:%02d\n", date.tm_year + 1900,
			date.tm_mon + 1, date.tm_mday, date.tm_hour, 
			date.tm_min, date.tm_sec);	
	record::file << tmp;
}

void
record::event_to_file(char *output)
{
	record::file << output;
}

void
record::close()
{
	record::file.close();	
}

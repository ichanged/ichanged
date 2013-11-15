#include "datum.h"
#include "logger.h"
#include "monitor.h"
#include "watcher.h"

FILE *datum::fp = NULL;
const char *datum::_filename = "datum";

void
datum::import_file()
{
	int wd;
	int length;
	struct stat s;
	char name[PATH_MAX + 1];

	// 从基准文件进行导入
	fp = fopen(datum::_filename, "rb");	
	while (true) {
		memset(name, 0, sizeof(name));
		if (!fread(&length, 2, 1, fp)) {
			break;
		}
		if (!fread(name, length, 1, fp)) {
			break;
		}
		if (!fread(&s, sizeof(s), 1, fp)) {
			break;
		}
		
		if (S_ISDIR(s.st_mode)) {
			wd = inotify_add_watch(monitor::inotify_fd, name, 
					monitor::mask);
			if(-1 == wd) {
				logger::warn("[%s %d]add watch to '%s' error: %s", 
						__FILE__, __LINE__, name,
						ERRSTR);
			}
			watcher::init_watch(wd, &s, name);	
		}
		if (S_ISREG(s.st_mode)) {
			watcher::init_file(&s, name);		
		}
	}
	watcher::print();
	fclose(fp);
}

void
datum::export_file()
{
	fp = fopen(datum::_filename, "wb");
	watcher::export_file();	
	fclose(fp);
}


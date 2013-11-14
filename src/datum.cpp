#include "datum.h"
#include "watcher.h"

FILE *datum::fp = NULL;
const char *datum::_filename = "datum";

//void
//datum::import_file()
//{
//	struct stat *s;
//
//	fp = fopen(datum::_filename, "rb");
//
//	while(fread(s, sizeof(struct stat), 1, fp)) {
//		if (S_ISDIR(s->st_mode)) {
//			printf("");	
//		}
//	}
//
//	fclose(fp);
//}

void
datum::export_file()
{
	fp = fopen(datum::_filename, "wb");
	watcher::export_file();	
	fclose(fp);
}


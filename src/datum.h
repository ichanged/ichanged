#ifndef _DATUM_H
#define _DATUM_H

#include <ftw.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <iostream>
#include <limits.h>

class datum {
public:
	static void import_file();
	static int compare(const char *fpath, const struct stat *sb, int typeflag); 
	static void export_file();
	static void deal_file(const char *fpath, const struct stat *sb); 
	static void deal_dir(const char *fpath, const struct stat *sb);

	static FILE *fp;
private:
	static const char *_filename;
};

#endif

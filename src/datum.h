#ifndef _DATUM_H
#define _DATUM_H

#include <iostream>
#include <stdio.h>
#include <limits.h>

class datum {
public:
	static void import_file();
	static void export_file();
	static FILE *fp;
private:
	static const char *_filename;
};

#endif

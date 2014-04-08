#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#define ICH_TRUE		1
#define ICH_FALSE		0

#define	ICH_STR_LEN		512

//#define ICH_IS_TOO_LONG(str)	{\
//	if (strlen(str) > 0) {\
//		printf("");
//	\}
//\}

extern void parse_args(int argc, char *argv[]);

#endif

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

#include "options.h"

std::string options::directory = "/home/liyiwei/ichanged";
uint32_t options::interval = 1;

void
options::parse_args(int argc, char *argv[])
{
	int opt;
	struct option opts[] = {
		{"interval", required_argument, NULL, 'i'},
		{"directory", required_argument, NULL, 'd'}
	};

	while((opt = getopt_long(argc, argv, "i:d:", opts, NULL)) != -1) {
		switch(opt) {
		case 'i':
			options::interval = atoi(optarg);
			break;
		case 'd':
			options::directory = std::string(optarg);
			break;
		case '?':
		case ':':
			options::print_usage();
			exit(EXIT_FAILURE);
			break;
		}
	}
}

void
options::print_usage()
{
	printf(
	"Usage: ichanged [OPTION...]\n"
	"a simple file system change monitor.\n"
	"\n"
	"Options:\n"
	"    -i, --interval        Monitor interval.\n"
	"    -d, --directory       Monitor directory.\n"
	);
}

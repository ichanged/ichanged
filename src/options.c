#include "log.h"
#include "config.h"
#include "options.h"


void parse_args();
static void print_usage(); 

void
ich_parse_args(int argc, char *argv[])
{
	int opt;

	struct options opts[] = {
		{"start", no_argument, NULL, "s"},
		{"end", no_argument, NULL, "e"},
	};

	while ((opt = getopt_long(argc, argv, "se", opts, NULL)) != -1) {
		switch (opt) {
		
		case 's':
			ich_deamon = ICH_TRUE;
			break;
		case 'e':
			ich_end = ICH_TRUE;	
			break;
		case '?':
		case ':':
			ich_daemon = ICH_FALSE;
			break;
		}	
	}
}

void
print_usage()
{
	printf(
	"Usage: ichanged [OPTION...]\n"
	"a simple file system change monitor.\n"
	"\n"
	"Options:\n"
	"    -s, --start	   start ichanged daemon process\n"
	"    -e, --end 		   ene ichanged daemon process\n"
	"open window to see changes since ichanged daemon process start\n")
}

#include "log.h"
#include "cfgig.h"
#include "options.h"


void parse_args();
static void print_usage(); 

void
ich_parse_args(int argc, char *argv[])
{
	int opt;
	struct option opts[] = {
		{"interval", required_argument, NULL, 'i'},
		{"directory", required_argument, NULL, 'd'},
		{"exclude", required_argument, NULL, 'e'},
		{"threshold", required_argument, NULL, 't'},
		{"long path", required_argument, NULL, 'l'},
		{"import datum file", required_argument, NULL, 'm'},
		{"export datum file", no_argument, NULL, 'x'},
		{"watch-hidden", no_argument, NULL, 'w'},
		{"start", no_argument, NULL, 's'}
	};

	while((opt = getopt_long(argc, argv, "i:d:e:t:l:mxws", opts, NULL)) != -1) {
		switch(opt) {
		case 'i':
			ich_cfg->intvl = atoi(optarg);
			break;
		case 'd':
			break;
		case 'e':
			break;	
		case 't':
			ich_cfg->threshold = atoi(optarg); 
			break;
		case 'l':
			break;
		case 'w':
			ich_cfg->hide = ICH_TRUE;
			break;
		case 'm':
			ich_cfg->import = ICH_TRUE;
			break;
		case 'x':
			ich_cfg->export = ICH_TRUE;
			break;
		case 's':
			ich_cfg->daemon = ICH_TRUE;	
			break;
		case '?':
		case ':':
			ich_print_usage();
			exit(EXIT_FAILURE);
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
	"    -i, --interval        Monitor interval, default is %ds.\n"
	"    -d, --directory       Monitor directory, default is '%s'.\n"
	"    -e, --exclude=PATH    Skip file or directory specified by PATH.\n"
	"    -t  --threshold	  Set threshold to show change when size of files beyonds it.\n"
	"    -l  --log path	  Set the path of log file\n"
	"    -w, --watch-hidden    Watch hidden files and directories.\n"
	"    -m  --import datum    import datum file 'datum'\n"
	"    -x  --export datum    export datum to file 'datum'\n",
	ich_cfg->intvl, ich_cfg->dir);
}

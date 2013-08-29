#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include "options.h"

/* 监控目录 */
std::string options::directory = "/var";
/* 界面刷新时间间隔 */
uint32_t options::interval = 2;
/* 排除监控的文件、目录 */
std::vector<std::string> options::exclude;
/* 是否监控隐藏文件、目录 */
bool options::watch_hidden = false;

void
options::parse_args(int argc, char *argv[])
{
	int opt;
	struct option opts[] = {
		{"interval", required_argument, NULL, 'i'},
		{"directory", required_argument, NULL, 'd'},
		{"exclude", required_argument, NULL, 'e'},
		{"watch-hidden", no_argument, NULL, 'w'}
	};

	while((opt = getopt_long(argc, argv, "i:d:e:w", opts, NULL)) != -1) {
		switch(opt) {
		case 'i':
			options::interval = atoi(optarg);
			break;
		case 'd':
			options::directory = std::string(optarg);
			break;
		case 'e':
			options::exclude.push_back(std::string(optarg));
			break;	
		case 'w':
			options::watch_hidden = true;
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
	"    -i, --interval        Monitor interval, default is %ds.\n"
	"    -d, --directory       Monitor directory, default is '%s'.\n"
	"    -e, --exclude=PATH    Skip file or directory specified by PATH.\n"
	"    -w, --watch-hidden    Watch hidden files and directories.\n",
	options::interval, options::directory.c_str());
}

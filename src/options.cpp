#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include "options.h"
#include "config.h"

/* 监控目录 */
std::string options::directory = "/tmp";
/* 界面刷新时间间隔 */
uint32_t options::interval = 2;
/* 排除监控的文件、目录 */
std::vector<std::string> options::exclude;
/* 是否监控隐藏文件、目录 */
bool options::watch_hidden = false;
/* 显示文件变化的阙值 */
long options::threshold = 0;
/* ichanged记录变化的文件 */
std::string options::filename = "change.log";
/* 日志记录的路径*/
std::string options::log_path = "ichanged.log";

void
options::parse_args(int argc, char *argv[])
{
	int opt;
	struct option opts[] = {
		{"interval", required_argument, NULL, 'i'},
		{"directory", required_argument, NULL, 'd'},
		{"exclude", required_argument, NULL, 'e'},
		{"threshold", required_argument, NULL, 't'},
		{"long path", required_argument, NULL, 'l'},
		{"watch-hidden", no_argument, NULL, 'w'},
		{"config file", no_argument, NULL, 'f'}
	};

	while((opt = getopt_long(argc, argv, "i:d:e:t:l:wf", opts, NULL)) != -1) {
		switch(opt) {
		case 'i':
			options::interval = atoi(optarg);
			break;
		case 'd':
			options::directory = std::string(optarg);
			break;
		case 'e':
			options::exclude.push_back(optarg);
			break;	
		case 't':
			options::threshold = atoi(optarg); 
			break;
		case 'l':
			options::log_path = std::string(optarg);
			break;
		case 'w':
			options::watch_hidden = true;
			break;
		case 'f':
			config::get_config();
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
	"    -t  --threshold	  Set threshold to show change when size of files beyonds it.\n"
	"    -l  --log path	  Set the path of log file\n"
	"    -w, --watch-hidden    Watch hidden files and directories.\n"
	"    -f  --config file	  Use configuration of config file ichanged.cfg\n",
	options::interval, options::directory.c_str());
}

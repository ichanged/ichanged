#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <string>
#include <inttypes.h>

class options {
public:
	static void parse_args(int argc, char *argv[]);
	static void print_usage();

	/* 监控目录 */
	static std::string directory;
	/* 界面刷新时间间隔 */
	static uint32_t interval;
	/* 排除监控的文件、目录 */
	static std::vector<std::string> exclude;
	/* 是否监控隐藏文件、目录 */
	static bool watch_hidden;
};

#endif

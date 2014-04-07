#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#define ICH_TRUE		1
#define ICH_FALSE		0


extern void parse_args(int argc, char *argv[]);

//#include <string>
//#include <vector>
//#include <inttypes.h>
//
//class options {
//public:
//	static void parse_args(int argc, char *argv[]);
//	static void print_usage();
//
//	/* 监控目录 */
//	static std::string directory;
//	/* 界面刷新时间间隔 */
//	static uint32_t interval;
//	/* 排除监控的文件、目录 */
//	static std::vector<std::string> exclude;
//	/* 是否监控隐藏文件、目录 */
//	static bool watch_hidden;
//	/* 显示文件变化的阙值 */
//	static long threshold;  
//	/* ichanged记录变化的文件 */
//	static std::string filename; 
//	/* 日志放置目录 */
//	static std::string log_path;
//	/* 导入基准文件*/
//	static bool is_import;
//	/* 导出基准文件*/
//	static bool is_export;
//
//	static bool is_daemon;
//	
//	static std::string pidfile;
//
//};

#endif

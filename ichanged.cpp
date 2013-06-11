#include "monitor.h"
#include "watcher.h"
#include "option.h"
#include "window.h"

int
main(int argc, char *argv[])
{
	/* 解析命令行参数 */
	option::parse_args(argc, argv);

	monitor::init();
	window::init();

	monitor::wait();
	window::wait();

	monitor::destory();
	window::destory();

	return 0;
}

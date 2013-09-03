#include "monitor.h"
#include "watcher.h"
#include "options.h"
#include "window.h"

int
main(int argc, char *argv[])
{
	/* 解析命令行参数 */
	options::parse_args(argc, argv);

	monitor::init();
	window::init();

	window::draw_event();

	monitor::wait();
	window::wait();

	monitor::destory();
//	window::destory();

	return 0;
}

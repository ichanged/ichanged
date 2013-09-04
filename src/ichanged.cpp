#include "monitor.h"
#include "watcher.h"
#include "options.h"
#include "window.h"

static void destroy();

void
destroy()
{
	printf("hello world\n");
	monitor::destroy();	
	//window::destroy();
}

int
main(int argc, char *argv[])
{
	int status;
	struct sigaction sa;

	memset(sa, 0, sizeof(sa));
	sa.sa_handler = destroy;
	status = sigaction(SIGINT, &sa, NULL);
	if (status == -1 ) {
		logger::warn("signal handler register error");
	}

	status = atexit(destory);
	if (status != 0) {
		logger::warn("exit handler register");
	}
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

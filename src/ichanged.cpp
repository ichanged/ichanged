#include <cstdlib>
#include <cstring>

#include "monitor.h"
#include "watcher.h"
#include "options.h"
#include "window.h"
#include "logger.h"

static void handler(int sig);
static void destroy();

void
handler(int sig)
{
	logger::info("[%s %d] ichanged stop waiting", __FILE__, __LINE__);	
	exit(EXIT_SUCCESS);
}

void
destroy()
{
	logger::info("[%s %d] ichanged exit normally", __FILE__, __LINE__);
	
	logger::destroy();
	monitor::destroy();	
	window::destroy();
}

int
main(int argc, char *argv[])
{
	int status;
	struct sigaction sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handler;
	status = sigaction(SIGINT, &sa, NULL);
	if (status == -1 ) {
		logger::warn("[%s %d]signal handler register error", __FILE__,
				__LINE__);
	}

	status = atexit(destroy);
	if (status != 0) {
		logger::warn("[%s %d]exit handler register", __FILE__, __LINE__);
	}

	/* 解析命令行参数 */
	options::parse_args(argc, argv);

	//logger::init();
	monitor::init();
	window::init();

	window::draw_event();

	monitor::wait();
	window::wait();

	return 0;
}

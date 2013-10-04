#include "monitor.h"
#include "watcher.h"
#include "options.h"
#include "window.h"
#include "record.h"
#include "logger.h"
#include "ichanged.h"

pthread_t tid[THREAD_NUM]; 
static void sigquit_handler(int sig);
static void sigint_handler(int sig);
static void destroy();

void
sigquit_handler(int sig)
{
	pthread_exit(NULL);
}

void
sigint_handler(int sig)
{
	int i;

	for(i = 0; i < THREAD_NUM; i++) {
		logger::info("[%s %d] thread: %lu will be killed", __FILE__,
				__LINE__, tid[i]);
		pthread_kill(tid[i], SIGQUIT);
	}
	logger::info("[%s %d] ichanged stop waiting", __FILE__, __LINE__);	
	exit(EXIT_SUCCESS);
}

void
destroy()
{
	monitor::destroy();	
	window::destroy();
	record::close();
	logger::info("[%s %d] ichanged exit normally", __FILE__, __LINE__);
	logger::destroy();
}

int
main(int argc, char *argv[])
{
	int status;
	struct sigaction sa_int, sa_quit;

	memset(&sa_int, 0, sizeof(sa_int));
	memset(&sa_quit, 0, sizeof(sa_quit));

	sa_int.sa_handler = sigint_handler;
	sa_quit.sa_handler = sigquit_handler;

	status = sigaction(SIGINT, &sa_int, NULL);
	if (status == -1 ) {
		logger::warn("[%s %d]signal handler register error", __FILE__,
				__LINE__);
	}
	status = sigaction(SIGQUIT, &sa_quit, NULL);
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

	logger::init();
	monitor::init();
	window::init();
	record::init();

	monitor::wait();
	window::wait();

	return 0;
}


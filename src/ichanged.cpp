#include "monitor.h"
#include "watcher.h"
#include "options.h"
#include "window.h"
#include "record.h"
#include "logger.h"
#include "error.h"
#include "config.h"
#include "ichanged.h"

	pthread_t tid[THREAD_NUM]; 
	int flag = 0;
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
		flag = 1;
	pthread_kill(tid[0], SIGQUIT);
	pthread_kill(tid[1], SIGQUIT);
//	for(i = 0; i < THREAD_NUM; i++) {
//		logger::info("[%s %d] thread: %lu will be killed", __FILE__,
//				__LINE__, tid[i]);
//		pthread_kill(tid[i], SIGQUIT);
//	}
//	logger::info("[%s %d] ichanged stop waiting", __FILE__, __LINE__);	
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

	try {
		status = sigaction(SIGINT, &sa_int, NULL);
		if (status == -1 ) {
			throw Error(__FILE__, __LINE__, 
					"signal handler register error");
		}
		status = sigaction(SIGQUIT, &sa_quit, NULL);
		if (status == -1 ) {
			throw Error(__FILE__, __LINE__,
					"signal handler register error");
		}
		status = atexit(destroy);
		if (status != 0) {
			throw Error(__FILE__, __LINE__,
					"exit function register error");
		}
	
		config::init();
		/* 解析命令行参数 */
		options::parse_args(argc, argv);
		
		logger::init();
		monitor::init();
		window::init();
		record::init();
	} catch(Error &x) {
		logger::fatal("[%s %d] %s", x.get_file(), x.get_line(), 
				x.what());
	}

	monitor::wait();
	window::wait();

	return 0;
}


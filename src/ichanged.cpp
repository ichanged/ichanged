#include "monitor.h"
#include "watcher.h"
#include "options.h"
#include "window.h"
#include "record.h"
#include "logger.h"
#include "error.h"
#include "config.h"
#include "ichanged.h"

static int ich_terminate; 
struct ich_sig_t ich_signals[] = {
	{SIGINT, ich_handler},
	{SIGCHLD, ich_handler}
};

void
ich_handler(int signo)
{
	switch (signo) {
	
	case SIGINT:
		sig_terminate = 1;
		break;
	case SIGQUIT:
		sig_terminate = 1;
		break;
	default:
		break;
	}
}


//pthread_t tid[THREAD_NUM]; 
//int flag = 0;
//static void sigquit_handler(int sig);
//static void sigint_handler(int sig);
//static void destroy();
//
//void
//sigquit_handler(int sig)
//{
//	pthread_exit(NULL);
//}
//
//void
//sigint_handler(int sig)
//{
//	flag = 1;
//	pthread_kill(tid[0], SIGQUIT);
//	pthread_kill(tid[1], SIGQUIT);
//	exit(EXIT_SUCCESS);
//}
//
//void
//destroy()
//{
//	monitor::destroy();	
//	window::destroy();
//	record::close();
//	logger::info("[%s %d] ichanged exit normally", __FILE__, __LINE__);
//	logger::destroy();
//}

int
main(int argc, char *argv[])
{
	int i;
	struct sigaction sa;

	config::init();
	options::parse_args(argc, argv);
	logger::init();

	memset(&sa, 0, sizeof(sa));	
	for (i = 0; i < sizeof(ich_signals) / sizeof(ich_signals[0]);
			i++) {
		sa.sa_handler = ich_signals[i].handler;
		if (sigaction(ich_signals[i].signo, &sa, NULL) < 0) {
			logger::error("[%s:%d] sigaction error: %s", __FILE__,
					__LINE__, ERRSTR);	
		}	
	}
	
	if (options::is_daemon) {
		ich_daemon_init();
	}

//	int status;
//	struct sigaction sa_int, sa_quit;
//
//	memset(&sa_int, 0, sizeof(sa_int));
//	memset(&sa_quit, 0, sizeof(sa_quit));
//
//	sa_int.sa_handler = sigint_handler;
//	sa_quit.sa_handler = sigquit_handler;
//
//	try {
//		status = sigaction(SIGINT, &sa_int, NULL);
//		if (status == -1 ) {
//			throw Error(__FILE__, __LINE__, 
//					"signal handler register error");
//		}
//		status = sigaction(SIGQUIT, &sa_quit, NULL);
//		if (status == -1 ) {
//			throw Error(__FILE__, __LINE__,
//					"signal handler register error");
//		}
//		status = atexit(destroy);
//		if (status != 0) {
//			throw Error(__FILE__, __LINE__,
//					"exit function register error");
//		}
//	
//		config::init();
//		/* 解析命令行参数 */
//		options::parse_args(argc, argv);
//		
//		logger::init();
//		record::init();
//		monitor::init();
////		window::init();
//	} catch(Error &x) {
//		logger::fatal("[%s %d] %s", x.get_file(), x.get_line(), 
//				x.what());
//	}
//
//	monitor::wait();
////	window::wait();

	return 0;
}


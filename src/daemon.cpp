#include "logger.h"
#include "monitor.h"
#include "daemon.h"
#include "config.h"
#include "options.h"
#include "record.h"

void
ich_daemon_init()
{
	FILE *fp = NULL;

	if (daemon(0, 0) < 0) {
		logger::fatal("[%s:%d] daemon error: %s", __FILE__, __LINE__,
				ERRSTR);	
	}	

	fp = fopen("/var/run/ichanged.pid", "w+");
	if (fp == NULL) {
		logger::fatal("[%s:%d] open pidfile error: %s", __FILE__,
				__LINE__, ERRSTR);	
	}
	fprintf(fp, "%d", getpid());		
	fclose(fp);

	// init
	record::init();
	monitor::init();
}

//void
//daemon::init()
//{
//	if (daemon(0, 0) < 0) {
//		logger::fatal("[%s:%d] daemon error: %s", __FILE__, __LINE__,
//				ERRSTR);	
//	}
//
//	daemon::write_pid();
//}
//
//void
//daemon::write_pid()
//{
//	FILE *fp = NULL;	
//
//	fp = fopen("/var/run/ichanged.pid", "w+");
//	if (fp == NULL) {
//		logger::fatal("[%s:%d] open pidfile error: %s", __FILE__,
//				__LINE__, ERRSTR);	
//	}
//	fprintf(fp, "%d", getpid());		
//
//	fclose(fp);
//}
//
//void
//daemon::read_pid()
//{
//	
//}

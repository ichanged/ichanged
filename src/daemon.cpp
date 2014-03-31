#include "log.h"
#include "daemon.h"
#include "config.h"
#include "options.h"

void
daemon::init()
{
	if (daemon(0, 0) < 0) {
		logger::fatal("[%s:%d] daemon error: %s", __FILE__, __LINE__,
				ERRSTR);	
	}

	daemon::write_pid();
}

void
daemon::write_pid()
{
	FILE *fp = NULL;	

	fp = fopen("/var/run/ichanged.pid", "w+");
	if (fp == NULL) {
		logger::fatal("[%s:%d] open pidfile error: %s", __FILE__,
				__LINE__, ERRSTR);	
	}
	fprintf(fp, "%d", getpid());		

	fclose(fp);
}

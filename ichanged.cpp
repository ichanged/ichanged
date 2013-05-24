#include "monitor.h"
#include "watcher.h"

int
main()
{
	g_watcher = new watcher();
	g_monitor = new monitor("/home/liyiwei/ichanged");

	g_monitor->start();
	return 0;
}

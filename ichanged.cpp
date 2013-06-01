#include "monitor.h"
#include "watcher.h"
#include "option.h"

int
main()
{
	g_watcher = new watcher();
	g_monitor = new monitor(option::directory);

	g_monitor->start();
	return 0;
}

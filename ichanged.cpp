#include "monitor.h"
#include "watcher.h"
#include "option.h"
#include "window.h"

int
main()
{
	monitor::init();
	window::init();

	monitor::wait();
	window::wait();

	monitor::destory();
	window::destory();

	return 0;
}

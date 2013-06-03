#include <ncurses.h>
#include <pthread.h>

#include "logger.h"
#include "window.h"

WINDOW *window::scr = NULL;
pthread_t window::thread_id = 0;

void
window::init()
{
	int status;

	scr = initscr();

	status = pthread_create(&window::thread_id, NULL, window::start, NULL);
	if(status != 0) {
		logger::fatal("create window thread error: %s", ERRSTR);
	}
}

void
window::wait()
{
	int status;
	void *ret;

	status = pthread_join(window::thread_id, &ret);
	if(status != 0) {
		logger::fatal("join window thread error: %s", ERRSTR);
	}
}

void
window::destory()
{
	endwin();
}

void *
window::start(void *arg)
{
	while(1) {
		window::refresh();
	}
	return NULL;
}

void
window::refresh()
{
	wprintw(scr, "ichanged - \n");
	wrefresh(scr);
	getch();
}

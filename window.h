#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <ncurses.h>
#include <pthread.h>

class window {
public:
	static void init();
	static void wait();
	static void destory();

	static void *start(void *arg);
	static void refresh();
private:
	static WINDOW *scr;
	static pthread_t thread_id;
};

#endif

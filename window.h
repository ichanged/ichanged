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

private:
	static void draw_summary();
	static void draw_event();

	static WINDOW *sum_win;
	static WINDOW *event_win;
	static pthread_t thread_id;

	static const int COL_MAX = 1024; 
};

#endif

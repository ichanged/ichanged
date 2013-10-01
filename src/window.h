#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <ncurses.h>
#include <pthread.h>
#include <string>
#include <signal.h>

class window {
public:
	static void init();
	static void wait();
	static void destroy();

	static void *start(void *arg);

private:
	static void draw_summary();
	static void draw_status_bar();
	static void draw_event();
	static void win_resize(int sig);

	static pthread_t thread_id;
	static std::string status_bar; 

	static const int COL_MAX = 1024; 
};

#endif

//hahahhahahhaha


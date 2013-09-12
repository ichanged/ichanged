#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <vector>

#include "logger.h"
#include "window.h"
#include "options.h"
#include "watcher.h"
#include "event.h"
#include "ichanged.h"

pthread_t window::thread_id = 0;
std::string window::status_bar;

void
window::init()
{
	int status;

	status = pthread_create(&window::thread_id, NULL, window::start, NULL);
	if(status != 0) {
		logger::fatal("create window thread error: %s", ERRSTR);
	}
	tid[1] = window::thread_id;
	logger::info("[%s %d] thread: %lu window module init completely", 
			__FILE__, __LINE__, window::thread_id);
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
window::destroy()
{
	endwin();
	logger::info("[%s %d] window module end completely", __FILE__, 
			__LINE__);
}

void *
window::start(void *arg)
{
//	struct sigaction sa;
//
//	initscr();
//
//	window::draw_summary();
//	window::draw_status_bar();
//
//	memset(&sa, 0, sizeof(struct sigaction));
//	sa.sa_handler = win_resize;
//
//	sigaction(SIGWINCH, &sa, NULL);
//
//	while(getch() != 27) {
//			
//	}
//	endwin();
//
	while (true) {
	//	window::draw_event();
	//	sleep(5);
	}
	return NULL;
}

void
window::win_resize(int sig)
{
	endwin();
	refresh();
	clear();

	window::draw_summary();
	window::draw_status_bar();

	refresh();
}

//		// 清除屏幕所有内容
//		wclear(window::scr);
//
//		// 打印摘要信息
//		window::draw_summary();
//		// 打印事件信息
//		window::draw_event();
//
//		// 将信息往终端上输出
//		wrefresh(window::scr);
//		// 根据配置的刷新时间，睡眠一段时间
//		sleep(options::interval);

void
window::draw_summary()
{
	struct tm result;
	time_t t;

	// 取当前时间
	t = time(NULL);
	localtime_r(&t, &result);

	printw("ichanged - %02d:%02d:%02d up\n",
		result.tm_hour, result.tm_min, result.tm_sec);
	printw("Directory: %s\n", options::directory.c_str());

	refresh();
//	wattron(window::scr, A_REVERSE);
//	wprintw(window::scr, " %-4s %-40s %-5s %-5s\n", 
//			"TYPE", "FILE", "BASE", "CUR");
//	wattroff(window::scr, A_REVERSE);
}

void
window::draw_status_bar()
{
	char tmp[60];

	sprintf(tmp, "%-4s %-40s %-5s %-5s", "TYPE", "FILE", "BASE", "CUR");
	window::status_bar = tmp;
	window::status_bar.resize(COLS, ' ');	
//	memset(str, 0, sizeof(str));
//	sprintf(str, "%-4s %-40s %-5s %-5s", "TYPE", "FILE", "BASE", "CUR");
//	memset(&str[strlen(str) - 1], ' ', COLS - (strlen(tmp) -1));
//	string[COLS] = '\0';
//		
	attron(A_REVERSE);
	printw("%s", window::status_bar.c_str());
	attroff(A_REVERSE);

	refresh();
}

void
window::draw_event()
{
	std::vector<event> *event_vec;
	std::vector<event>::iterator pos;

	watcher::lock();
	event_vec = watcher::generate_snapshot();
	watcher::unlock();
	for(pos = event_vec->begin(); pos != event_vec->end(); ++pos) {
//		wprintw(window::scr, " %-4s %-40s %-5d %-5d\n",
//			pos->get_type_string().c_str(), pos->get_path().c_str(),
//			pos->get_base_size(), pos->get_current_size());
		printf(" %-4s %-40s %-5ld %-5ld\n",
			pos->get_type_string().c_str(), pos->get_path().c_str(),
			pos->get_base_size(), pos->get_current_size());
	}
}

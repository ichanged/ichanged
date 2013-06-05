#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <vector>

#include "logger.h"
#include "window.h"
#include "option.h"
#include "watcher.h"
#include "event.h"

WINDOW *window::scr = NULL;
pthread_t window::thread_id = 0;

void
window::init()
{
	int status;

	window::scr = initscr();

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
	while(true) {
		// 清除屏幕所有内容
		wclear(window::scr);

		// 打印摘要信息
		window::draw_summary();
		// 打印事件信息
		window::draw_event();

		// 将信息往终端上输出
		wrefresh(window::scr);
		// 根据配置的刷新时间，睡眠一段时间
		sleep(option::interval);
	}
	return NULL;
}

void
window::draw_summary()
{
	struct tm result;
	time_t t;

	// 取当前时间
	t = time(NULL);
	localtime_r(&t, &result);

	wprintw(window::scr, "ichanged - %02d:%02d:%02d up\n",
		result.tm_hour, result.tm_min, result.tm_sec);
	wprintw(window::scr, "Directory: %s\n", option::directory.c_str());
	wattron(window::scr, A_REVERSE);
	wprintw(window::scr, " %-4s %-30s %-5s %-5s\n", "TYPE", "FILE", "BASE", "CUR");
	wattroff(window::scr, A_REVERSE);
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
		wprintw(window::scr, " %-4s %-30s %-5d %-5d\n",
			pos->get_type_string().c_str(), pos->get_path().c_str(),
			pos->get_base_size(), pos->get_current_size());
	}
}

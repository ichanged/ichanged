#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <vector>

#include "logger.h"
#include "error.h"
#include "window.h"
#include "options.h"
#include "watcher.h"
#include "event.h"
#include "ichanged.h"
#include "record.h"

pthread_t window::thread_id = 0;
std::string window::status_bar;
WINDOW *window::status;
WINDOW *window::event_list;

void
window::init()
{
	int status;

	status = pthread_create(&window::thread_id, NULL, window::start, NULL);
	if(status != 0) {
		throw Error(__FILE__, __LINE__, 
				"create window thread error: %s", ERRSTR);
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

	try {
		status = pthread_join(window::thread_id, &ret);
		if(status != 0) {
			logger::fatal("join window thread error: %s", ERRSTR);
		}
	} catch(...) {
		endwin();
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
	struct sigaction sa;

	initscr();

	window::status = newwin(3, 0, 0, 0);
	window::event_list = newwin(LINES - 3, 0, 3, 0);

	window::draw_summary();
	window::draw_status_bar();

	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = win_resize;

	sigaction(SIGWINCH, &sa, NULL);

	while (!flag) {
		wclear(window::event_list);
		wrefresh(window::event_list);
		window::draw_event();
		wrefresh(window::event_list);
		sleep(options::interval);
	}

	endwin();
	
	return NULL;
}

void
window::win_resize(int sig)
{
	endwin();
	wclear(window::status);
	wrefresh(window::status);
	window::draw_summary();
	window::draw_status_bar();
	wrefresh(window::status);
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

	wprintw(window::status, "ichanged - %02d:%02d:%02d up\n",
		result.tm_hour, result.tm_min, result.tm_sec);
	wprintw(window::status, "Directory: %s\n", options::directory.c_str());

	wrefresh(window::status);
//	wattron(window::scr, A_REVERSE);
//	wprintw(window::scr, " %-4s %-40s %-5s %-5s\n", 
//			"TYPE", "FILE", "BASE", "CUR");
//	wattroff(window::scr, A_REVERSE);
}

void
window::draw_status_bar()
{
	char tmp[1000];

	sprintf(tmp, "%-5s  %-8s  %-8s   %-22s %-5s", "TYPE", "BASE", "CUR", 
			"TIME", "FILE");
	window::status_bar = tmp;
	window::status_bar.resize(COLS, ' ');	
//	memset(str, 0, sizeof(str));
//	sprintf(str, "%-4s %-40s %-5s %-5s", "TYPE", "FILE", "BASE", "CUR");
//	memset(&str[strlen(str) - 1], ' ', COLS - (strlen(tmp) -1));
//	string[COLS] = '\0';
//		
	wattron(window::status,A_REVERSE);
	wprintw(window::status, "%s", window::status_bar.c_str());
	wattroff(window::status, A_REVERSE);

	wrefresh(window::status);
}

void
window::_itoa(off_t size, std::string &size_str)
{
	std::stringstream ss;

	if (size == -2) {
		ss << "--";
	} else {
		ss << size;
	}
	size_str = ss.str();
}

void
window::draw_event()
{
	std::vector<event> *event_vec;
	std::vector<event>::iterator pos;
	std::string base_size, current_size;
	char output[1000] = {0};

	watcher::lock();
      	event_vec = watcher::generate_snapshot();
	watcher::unlock();

	record::get_time();
	for(pos = event_vec->begin(); pos != event_vec->end(); ++pos) {
		if (abs(pos->get_base_size() - pos->get_current_size())
				< options::threshold) { 
			if (pos->get_base_size() != -2 && 
				pos->get_current_size() != -2) {
				continue;
			}
		} 
		window::_itoa(pos->get_base_size(), base_size);
		window::_itoa(pos->get_current_size(), current_size);

		sprintf(output, " %-5s %-8s %-8s %-22s %-20s\n", 
				pos->get_type_string().c_str(), 
				base_size.c_str(), current_size.c_str(),
				pos->get_chg_time(), pos->get_path().c_str());		
		if (record::change_flag) {	
			record::event_to_file(output);
		}
		wprintw(window::event_list, output);
		wrefresh(window::event_list);
	}
}

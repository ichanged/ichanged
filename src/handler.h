#ifndef _HANDLER_H_
#define _HANDLER_H_

#include <sys/inotify.h>
#include <unistd.h>
#include <string>

class handler {
public:
	static void handle_event(struct inotify_event *e);
	static void handle_dir_event(struct inotify_event *e, std::string path);
	static void handle_file_event(struct inotify_event *e, std::string path);
};

#endif

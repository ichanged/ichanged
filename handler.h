#ifndef _HANDLER_H_
#define _HANDLER_H_

#include <sys/inotify.h>
#include <string>

class handler {
public:
	static void handle_event(struct inotify_event *e);
};

#endif

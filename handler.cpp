#include "monitor.h"
#include "handler.h"
#include "watcher.h"
#include "logger.h"

void
handler::handle_event(struct inotify_event *e)
{
	watch w = watcher::get_watch(e->wd);
	std::string path;

	path = w.get_path();
	if(!(e->mask & IN_DELETE_SELF) && !(e->mask & IN_MOVE_SELF)) {
		if(e->len > 0) {
			path += "/";
			path += e->name;
		}
	}
	/* 如果有新的目录被创建或者被移入监控目录，则对新目录加监控 */
	if((e->mask & IN_CREATE && e->mask & IN_ISDIR)
	|| (e->mask & IN_MOVED_TO && e->mask & IN_ISDIR)) {
		g_monitor->add_monitor(path);
	}
	/* 如果有目录被从监控目录中移出或者被删除，则删除监控 */
	if(e->mask & IN_IGNORED) {
		g_monitor->remove_monitor(e->wd);
	}

	/* 如果有新的普通文件被创建，则向监控下添加文件 */
	if(e->mask & IN_CREATE && !(e->mask & IN_ISDIR)) {
		watcher::file_create(e->wd, e->name);
	}
	/* 如果已有的文件权限被改变，则记录新的权限 */
	if(e->mask & IN_ATTRIB) {
		if(e->mask & IN_ISDIR) {
			watcher::dir_attrib(e->wd, e->name);
		} else {
			watcher::file_attrib(e->wd, e->name);
		}
	}
	watcher::print_diff_result();
}

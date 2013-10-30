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
	if (!(e->mask & IN_DELETE_SELF) && !(e->mask & IN_MOVE_SELF)) {
		if (e->len > 0) {
			path += "/";
			path += e->name;
		}
	}

	watcher::lock();
	if (e->mask & IN_ISDIR) {
		handler::handle_dir_event(e, path);
	} else {
		handler::handle_file_event(e, path);
	}
	watcher::unlock();
}

void
handler::handle_dir_event(struct inotify_event *e, std::string path)
{
	if (monitor::is_path_hidden(path.c_str())) {
		return;	
	}
	/* 如果有新的目录被创建或者被移入监控目录，则对新目录加监控 */
	if (e->mask & IN_CREATE || e->mask & IN_MOVED_TO) {
		//printf("%s dir was created\n", e->name);
		monitor::add_monitor(path);
	}
	/* 如果有目录被从监控目录中移出或者被删除，则删除监控 */
	if (e->mask & IN_IGNORED || e->mask & IN_DELETE) {
		//printf("%s dir was ignored\n", e->name);
		watcher::dir_delete(e->wd, e->name);
	}
	/* 如果已有的目录权限被改变，则记录新的权限 */
	if (e->mask & IN_ATTRIB) {
		//printf("%s dir was attribed\n", e->name);
		watcher::dir_attrib(e->wd, e->name);
	}
	if (e->mask & IN_MODIFY) {
		//printf("%s dir was modified\n", e->name);
		watcher::dir_modify(e->wd, e->name);
	}
}

//void
//handler::handle_file_event(struct inotify_event *e, std::string path)
//{
//	struct stat buf;
//	/* 如果有新的普通文件被创建，则向监控下添加文件 */
//	if (e->mask & IN_CREATE) {
//		printf("%s is created\n", path.c_str());
//		if (stat(path.c_str(), &buf) == -1) {
//			printf("ERROR when try to get stat ---create file");	
//		}	
//		//watcher::file_create(e->wd, e->name);
//	}
//	/* 如果已有的文件权限被改变，则记录新的权限 */
//	if (e->mask & IN_ATTRIB) {
//		printf("%s is attribed\n", path.c_str());
//		if (stat(path.c_str(), &buf) == -1) {
//			printf("ERROR when try to get stat ---attrib file");	
//		}	
//		//watcher::file_attrib(e->wd, e->name);
//	}
//	/* 如果已有的文件被修改，则记录文件新的属性 */
//	if (e->mask & IN_MODIFY) {
//		printf("%s is modified\n", path.c_str());
//		if (stat(path.c_str(), &buf) == -1) {
//			printf("ERROR when try to get stat ---modify file");	
//		}	
//		//watcher::file_modify(e->wd, e->name);
//	}
//	if (e->mask & IN_DELETE || e->mask & IN_MOVE) {
//		printf("%s is deleted\n", path.c_str());
//		if (stat(path.c_str(), &buf) == -1) {
//			printf("ERROR when try to get stat ---delete file");	
//		}	
//		//watcher/file_delete/:e->:wde->(,name);
//	}
//}

void
handler::handle_file_event(struct inotify_event *e, std::string path)
{
	if (monitor::is_path_hidden(path.c_str())) {
		return;
	}	
	if (e->mask & IN_CREATE) {
		//printf("%s was created\n", e->name);
		watcher::file_create(e->wd, e->name);		
	}
	if (e->mask & IN_ATTRIB) {
		//printf("%s was attribed\n", e->name);
		watcher::file_attrib(e->wd, e->name);
	}
	if (e->mask & IN_DELETE || e->mask & IN_MOVED_FROM) {
		//printf("%s was deleted\n", e->name);
		watcher::file_delete(e->wd, e->name);
	}
	if (e->mask & IN_MODIFY) {
		//printf("%s was modified\n", e->name);
		watcher::file_modify(e->wd, e->name);
	} else if (e->mask & IN_CLOSE_WRITE) {
		//printf("%s was write\n", e->name);
		watcher::file_write(e->wd, e->name);
	}
	//printf("%s %d\n", e->name, e->mask);
}

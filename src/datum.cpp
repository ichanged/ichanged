#include "datum.h"
#include "logger.h"
#include "monitor.h"
#include "options.h"
#include "watcher.h"

FILE *datum::fp = NULL;
const char *datum::_filename = "datum";

void
datum::import_file()
{
	int wd;
	int ret;
	int length;
	struct stat s;
	char name[PATH_MAX + 1];

	// 从基准文件进行导入
	fp = fopen(datum::_filename, "rb");	
	while (true) {
		memset(name, 0, sizeof(name));
		if (!fread(&length, 2, 1, fp)) {
			break;
		}
		if (!fread(name, length, 1, fp)) {
			break;
		}
		if (!fread(&s, sizeof(s), 1, fp)) {
			break;
		}
		
		if (S_ISDIR(s.st_mode)) {
			wd = inotify_add_watch(monitor::inotify_fd, name, 
					monitor::mask);
			if(-1 == wd) {
				logger::warn("[%s %d]add watch to '%s' error: %s", 
						__FILE__, __LINE__, name,
						ERRSTR);
			}
			watcher::init_watch(wd, &s, name);	
		}
		if (S_ISREG(s.st_mode)) {
			watcher::init_file(&s, name);		
		}
	}
	fclose(fp);

	//watcher::print();
	// 与当前实际文件情况比较，输出不同	
	ret = ftw(options::directory.c_str(), (ftw_func)datum::compare, 500);
	if(ret == -1) {
		logger::fatal("traverse directory '%s' to add monitor error",
			options::directory.c_str());
	}

	watcher::check_delete();
}

int
datum::compare(const char *fpath, const struct stat *sb, int typeflag)
{
	switch (typeflag) {
	case FTW_F:
		datum::deal_file(fpath, sb);
		break;
	case FTW_D:
		datum::deal_dir(fpath, sb);
		break;
	}

	return 0;
}

void
datum::export_file()
{
	fp = fopen(datum::_filename, "wb");
	watcher::export_file();	
	fclose(fp);
}

void
datum::deal_file(const char *fpath, const struct stat *sb)
{
	int wd;
	struct stat ns;
	char buf[PATH_MAX] = {0};
	struct stat sb_link;
	watch *w_tmp = NULL;
	file *f_tmp = NULL;
	char *dir = NULL;
	char *filename = NULL;

	char *dbuf = new char[strlen(fpath) + 1];
	char *fbuf = new char[strlen(fpath) + 1];
	strcpy(dbuf, fpath);
	dir = dirname(dbuf);
	strcpy(fbuf, fpath);
	filename = basename(fbuf);
		
	wd = watcher::_wd_map[dir];
	w_tmp = watcher::get_watch(wd);
	f_tmp = w_tmp->get_file(filename);
	w_tmp->set_read(true);
	
	if (w_tmp->is_file_exist(filename)) {
		ns = f_tmp->get_base();
		f_tmp->set_read(true);
		if (sb->st_size != ns.st_size) {
			watcher::file_modify(wd, filename);
		}
		if (sb->st_mode != ns.st_mode) {
			watcher::file_attrib(wd, filename);
		}

		if (lstat(fpath, &sb_link) == -1) {
			logger::warn("[%s %d]lstat error: %s", __FILE__, 
					__LINE__, ERRSTR);			
		}
		if (S_ISLNK(sb_link.st_mode)) {
			if (readlink(fpath, buf, sizeof(buf)) == -1) {
				logger::fatal("[%s %d]readlink error: %s", 
						__FILE__, __LINE__, ERRSTR);
			}
			if (stat(buf, &sb_link)) {
				logger::warn("[%s %d]lstat error: %s", __FILE__, 
						__LINE__, ERRSTR);			
			}
			datum::deal_file(buf, &sb_link);
		}
	} else {
		watcher::add_file(sb, fpath);	
	}

	delete dbuf;
	delete fbuf;
}

void
datum::deal_dir(const char *fpath, const struct stat *sb)
{
	int wd;
	struct stat ns;
	std::string dir;
	watch *w_tmp;

	dir.assign(fpath);
	if (!watcher::is_watch_exist(dir)) {
		wd = inotify_add_watch(monitor::inotify_fd, fpath, monitor::mask);
		if (-1 == wd) {
			logger::warn("add watch to '%s' error", fpath);
		}
		watcher::add_watch(wd, sb, fpath);
	} else {
		wd = watcher::_wd_map[dir];
		w_tmp = watcher::get_watch(wd);
		ns = w_tmp->get_base();
		w_tmp->set_read(true);

		if (ns.st_size != sb->st_size) {
			watcher::dir_modify(wd, (char *)fpath);	
		}
		if (ns.st_mode != sb->st_mode) {
			watcher::dir_attrib(wd, (char *)fpath);
		}
	}
}

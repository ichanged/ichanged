#include <iostream>
#include <cstdio>
#include <errno.h>
#include <cstdlib>
#include <sys/inotify.h>
#include <ftw.h>

const EVENT_MAX = sizeof(struct inotify_event) + MAX + 1;
const std::string directory = "/home/faye/ichanged"; 


int inotify_fd;
int mask;
std::vector<int> wd_array; 

typedef int (*ftw_func) (const char *fpath, const struct stat *sb, 
		int typeflag);

int
add_monitor(const char *fpath, const struct stat *sb, int typeflag)
{
	int wd;

	if (typeflag == FTW_D) {
		wd = inotify_add_watch(inotify_fd, fpath, mask);
	}

	return 0;
}

int
main(int argc, char *argv[])
{
	int retval;
	mask = IN_CREATE | IN_ATTRIB | IN_MODIFY | IN_DELETE;

	inotify_fd = inotify_init();

	retval = ftw(directory.c_str(), (ftw_func)add_monitor, 500);
	if (retval == -1) {
		perror("ftw error");
		exit(EXIT_FAILURE);
	}

	while (true) {
			
	}

	return 0;	
}

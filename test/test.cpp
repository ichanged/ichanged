#include <iostream>
#include <errno.h>
#include <cstdlib>
#include <sys/inotify.h>

const string directory = "/home/wufei/ichanged"; 

typedef int (*ftw_func) (const char *fpath, const struct stat *sb, 
		int typeflag);

int
add_monitor(const char *fpath, const struct stat *sb, int typeflag)
{
	
}

int
main(int argc, char *argv[])
{
	int inotify_fd;
	int retval;
	int mask = IN_CREATE | IN_ATTRIB | IN_MODIFT | IN_DELETE;

	inotify_fd = inotify_init();

	retval = ftw(directory.c_str(), (ftw_func)add_monitor);
	if (retval == -1) {
		perror("ftw error");
		exit(EXIT_FAILURE);
	}

	return 0;	
}

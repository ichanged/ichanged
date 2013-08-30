#include <iostream>
#include <cstring>
#include <unistd.h>
#include <cstdio>
#include <errno.h>
#include <cstdlib>
#include <sys/inotify.h>
#include <ftw.h>
#include <vector>
#include <map>
#include <utility>
#include <limits.h>

const int EVENT_MAX = sizeof(struct inotify_event) + NAME_MAX + 1;
const std::string directory = "/tmp/xxtest/"; 


int inotify_fd;
int mask;
std::map<std::string, int> watch;

typedef int (*ftw_func) (const char *fpath, const struct stat *sb, 
		int typeflag);

int
add_monitor(const char *fpath, const struct stat *sb, int typeflag)
{
	int wd;

	if (typeflag == FTW_D) {
		wd = inotify_add_watch(inotify_fd, fpath, mask);
		watch.insert(std::pair<std::string, int>(fpath, wd));
	}

	return 0;
}

void
handle(struct inotify_event *event)
{
	std::string path 

	std::map<std::string, int>::iterator iter;	
	iter = watch.find(event->wd);
	if iter == watch.end() {
		printf("find watch error!\n");
		exit(EXIT_FAILURE);
	}

	printf("%");
}

int
main(int argc, char *argv[])
{
	int num, size;
	int retval;
	char buf[EVENT_MAX] = {0};
	struct inotify_event *tmp = NULL, *event = NULL;
	mask = IN_CREATE | IN_ATTRIB | IN_MODIFY | IN_DELETE;

	inotify_fd = inotify_init();

	retval = ftw(directory.c_str(), (ftw_func)add_monitor, 500);
	if (retval == -1) {
		perror("ftw error");
		exit(EXIT_FAILURE);
	}

	while ((num = read(inotify_fd, buf, EVENT_MAX)) != 0) {
		if (num < 0) {
			perror("read error");
			exit(EXIT_FAILURE);
		}	
		
		tmp = (struct inotify_event *)buf;
		size = sizeof(struct inotify_event) + tmp->len;	
		event = (struct inotify_event *)malloc(size);
		memcpy(event, buf, size);

		handle(event);
			
	}

	return 0;	
}

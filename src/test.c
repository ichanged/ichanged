#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/inotify.h>


int
main(void)
{
	int fd, i, tmp, len;
	int wd[5];
	char buf[1024] = {0};
	char *buffer = NULL;
	struct inotify_event *event;

	char *monitor_files[] = {
		"/home/wufei/ichanged",
		"/home/wufei/Source/SAS-H/wufei"
	};
	char *event_array[] = {
		"File was accessed",
		"File was modified",
		"File attributes were changed",
		"writtable file closed",
		"Unwrittable file closed",
		"File was opened",
		"File was moved from X",
		"File was moved to Y",
		"Subfile was created",
		"Subfile was deleted",
		"Self was deleted",
		"Self was moved",
		"",
		"Backing fs was unmounted",
		"Event queued overflowed",
		"File was ignored"
	};

	fd = inotify_init();
	if (fd == -1) {
		perror("inotify init error");
		exit(EXIT_FAILURE);
	}

	for(i = 0; i < 2; i++) {
		tmp = inotify_add_watch(fd, monitor_files[i], IN_ALL_EVENTS);	
		if (tmp == -1) {
			perror("add watch error");		
		}

		wd[i] = tmp;
	}

	while((len = read(fd, buf, 1024)) > 0) {
		buffer = buf;
		event = (struct inotify_event *)buf;

		printf("0x%p", event);
		printf("0x%p", buf);
//		if (event->mask & IN_ISDIR) {
//			printf("Object Type: Dictionary\n");
//		} else {
//			printf("Object Type: File\n");	
//		}	
//
//		for(i = 0; i < 2; i++) {
//			if (event->wd == wd[i]) {
//				printf("Object Name: %s\n", monitor_files[i]);	
//			}	
//		}
//		for(i = 0; i < 16; i++) {
//			if (event->mask & (1 << i)) {
//				printf("Object content: %s\n", event_array[i]);	
//			}
//		}
	}

	return 0;
}

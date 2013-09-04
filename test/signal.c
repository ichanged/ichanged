#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

void *
new_thread(void *arg)
{
	printf("This is new thread!\n");
	sleep(10);
	return NULL;
}

void
handle_sigint(int sig)
{
	pthread_t tid = pthread_self();

	printf("thread %u exit\n", (unsigned int)tid);	
	pthread_exit(NULL);
}

int
main()
{
	pthread_t thread;
	struct sigaction sa;

	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = handle_sigint; 

	sigaction(SIGINT, &sa, NULL);
	pthread_create(&thread, NULL, new_thread, NULL);
	printf("This is main thread!\n");

	pthread_exit(NULL);

	return 0;
}

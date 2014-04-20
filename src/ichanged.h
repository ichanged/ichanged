#ifndef _ICHANGED_H_
#define _ICHANGED_H_

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define THREAD_NUM	2

extern pthread_t tid[THREAD_NUM];
extern int flag; 

#endif

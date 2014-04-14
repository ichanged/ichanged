#ifndef _LOG_H
#define _LOG_H

#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

/* 日志级别 */
#define LOG_FATAL	(4)
#define LOG_ERROR	(3)
#define LOG_WARN	(2)
#define LOG_INFO	(1)
#define LOG_DEBUG	(0)

#define ich_LOG_PATH 		"ich_error.log"

extern void ich_log_init();
extern void ich_log_end();
extern void ich_log_debug(const char *format, ...);
extern void ich_log_info(const char *format, ...);
extern void ich_log_warn(const char *format, ...);
extern void ich_log_error(const char *format, ...);
extern void ich_log_fatal(const char *format, ...);

#endif

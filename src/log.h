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

#define SWS_LOG_PATH 		"ich_error.log"

extern void SWS_log_init();
extern void SWS_log_end();
extern void SWS_log_debug(const char *format, ...);
extern void SWS_log_info(const char *format, ...);
extern void SWS_log_warn(const char *format, ...);
extern void SWS_log_error(const char *format, ...);
extern void SWS_log_fatal(const char *format, ...);

#endif

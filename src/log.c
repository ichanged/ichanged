#include "log.h"

static void ich_log_to_file(int level, const char *log);

static void ich_err_exit1(const char *format, ...);

/* 日志文件的文件指针 */
static FILE *ich_log_fp = NULL;
/* 日志文件的文件描述符 */
static int ich_log_fd = 0;

void
ich_err_exit1(const char *format, ...)
{
	va_list ap;
	char buf[1024];

	va_start(ap, format);
	vsprintf(buf, format, ap);
	fprintf(stderr, "%s: %s\n", buf, strerror(errno));
	va_end(ap);
	exit(EXIT_FAILURE);
}

/**
 * 初始化日志模块
 */
void
ich_log_init()
{
#ifdef ich_DEBUG
	ich_log_fp = stdout;
#else
	ich_log_fp = fopen(ich_LOG_PATH, "a");
#endif
	if(ich_log_fp == NULL) {
		ich_err_exit1("[%s %d] open log file '%s' error", __FILE__, __LINE__,
			ich_LOG_PATH);
	}
	ich_log_fd = fileno(ich_log_fp);
	if(ich_log_fd == -1) {
		ich_err_exit1("[%s %d] get log file stream fd error", __FILE__,
			__LINE__);
	}
}

/**
 * 结束日志记录
 */
void
ich_log_end()
{
	if(ich_log_fp != NULL) {
		fclose(ich_log_fp);
	}
}

/**
 * 记录调试信息，程序可以继续执行
 * @param format 格式化字符串
 * @param 可变的填充格式化字符串使用的参数
 */
void
ich_log_debug(const char *format, ...)
{
	char log_buf[1024] = {0};
	va_list ap;

	va_start(ap, format);
	vsnprintf(log_buf, sizeof(log_buf), format, ap);
	va_end(ap);
	ich_log_to_file(LOG_DEBUG, log_buf);
}

/**
 * 记录普通信息，程序可以继续执行
 * @param format 格式化字符串
 * @param 可变的填充格式化字符串使用的参数
 */
void
ich_log_info(const char *format, ...)
{
	va_list ap;
	char log_buf[1024] = {0};

	va_start(ap, format);
	vsnprintf(log_buf, sizeof(log_buf), format, ap);
	va_end(ap);
	ich_log_to_file(LOG_INFO, log_buf);
}

/**
 * 记录警告信息，程序可以继续执行
 * @param format 格式化字符串
 * @param 可变的填充格式化字符串使用的参数
 */
void
ich_log_warn(const char *format, ...)
{
	va_list ap;
	char log_buf[1024] = {0};

	va_start(ap, format);
	vsnprintf(log_buf, sizeof(log_buf), format, ap);
	va_end(ap);
	ich_log_to_file(LOG_WARN, log_buf);
}

/**
 * 记录错误，记录后就错误退出
 * @param format 格式化字符串
 * @param 可变的填充格式化字符串使用的参数
 */
void
ich_log_error(const char *format, ...)
{
	va_list ap;
	char log_buf[1024] = {0};

	va_start(ap, format);
	vsnprintf(log_buf, sizeof(log_buf), format, ap);
	va_end(ap);
	ich_log_to_file(LOG_ERROR, log_buf);
//	exit(EXIT_FAILURE);
}

/**
 * 记录致命错误，记录后就错误退出
 * @param format 格式化字符串
 * @param 可变的填充格式化字符串使用的参数
 */
void
ich_log_fatal(const char *format, ...)
{
	va_list ap;
	char log_buf[1024] = {0};

	va_start(ap, format);
	vsnprintf(log_buf, sizeof(log_buf), format, ap);
	va_end(ap);
	ich_log_to_file(LOG_FATAL, log_buf);
	exit(EXIT_FAILURE);
}


/**
 * 将日志语句写入文件
 * @param level 日志等级
 * @param log 日志字符串
 */
static void
ich_log_to_file(int level, const char *log)
{
	time_t t;
	struct tm date;
	struct stat sb;
	const char *level_str = NULL;

	switch(level) {
	case LOG_FATAL:
		level_str = "[FATAL]";
		break;
	case LOG_ERROR:
		level_str = "[ERROR]";
		break;
	case LOG_WARN:
		level_str = "[WARN]";
		break;
	case LOG_INFO:
		level_str = "[INFO]";
		break;
	case LOG_DEBUG:
		level_str = "[DEBUG]";
		break;
	default:
		return;
	}

	/* 对日志文件加锁 */
	if(-1 == lockf(ich_log_fd, F_LOCK, 0)) {
		ich_err_exit1("[%s %d] lock file '%s' error", __FILE__, __LINE__,
			ich_LOG_PATH);
	}

	/* 如果日志文件超出大小限制则清空 */
	if(-1 == fstat(ich_log_fd, &sb)) {
		ich_err_exit1("[%s %d] get log file '%s' status error", __FILE__, __LINE__,
			ich_LOG_PATH);
	}
	if(sb.st_size > 10 * 1024 * 1024) {
		if(-1 == ftruncate(ich_log_fd, 0)) {
			ich_err_exit1("[%s %d] truncate file '%s' error", __FILE__, __LINE__,
				ich_LOG_PATH);
		}
	}

	t = time(NULL);
	localtime_r(&t, &date);
	/* 打印日期时间 */
	fprintf(ich_log_fp, "[%d/%02d/%02d %02d:%02d:%02d] ",
		date.tm_year + 1900, date.tm_mon + 1, date.tm_mday,
		date.tm_hour, date.tm_min, date.tm_sec);
	/* 打印日志内容 */
	fprintf(ich_log_fp, "%s %s\n", level_str, log);
	fflush(ich_log_fp);

	/* 对日志文件解锁 */
	if(-1 == lockf(ich_log_fd, F_ULOCK, 0)) {
		ich_err_exit1("[%s %d] unlock file '%s' error", __FILE__, __LINE__,
			ich_LOG_PATH);
	}
}

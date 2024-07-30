#ifndef __ATLIB_LOG_H
#define __ATLIB_LOG_H

#include "Atlib/bufwrite.h"

#define ATLIB_ENVIRONMENT_VARIABLE_MAX_LEN 2048

typedef enum {
    ATLIB_LOG_DEBUG = 0,
    ATLIB_LOG_INFO,
    ATLIB_LOG_WARN,
    ATLIB_LOG_ERROR,
    ATLIB_LOG_FATAL = 255,
} log_level_e;

typedef struct {
    bufwrite_t bw;
    log_level_e min;
} log_t;

extern log_level_e atlib_log_level(const char * level_name);
extern log_t * atlib_log_open(log_t *__restrict log, const char *__restrict file_name);
extern log_t * atlib_log_fopen(log_t *__restrict log, FILE *__restrict file);
extern void atlib_log_close(log_t * log);

extern void atlib_log_writef(log_t *__restrict log,
        const char *__restrict level, const char *__restrict file, i32 line,
        const char *__restrict fmt, ...) __attribute__ ((format (printf,5,6)));

#define atlib_log_debug(log, fmt, ...) atlib_log_writef(log, "DEBUG", __FILE__, __LINE__, fmt, __VA_ARGS__)
#define atlib_log_info(log, fmt, ...) atlib_log_writef(log, "INFO", __FILE__, __LINE__, fmt, __VA_ARGS__)
#define atlib_log_warn(log, fmt, ...) atlib_log_writef(log, "WARN", __FILE__, __LINE__, fmt, __VA_ARGS__)
#define atlib_log_error(log, fmt, ...) atlib_log_writef(log, "ERROR", __FILE__, __LINE__, fmt, __VA_ARGS__)
#define atlib_log_fatal(log, fmt, ...) atlib_log_writef(log, "FATAL", __FILE__, __LINE__, fmt, __VA_ARGS__)


#ifdef __DEBUG__
#  define atlib_dbglog_debug(log, fmt, ...) atlib_log_writef(log, "DEBUG", __FILE__, __LINE__, fmt, __VA_ARGS__)
#  define atlib_dbglog_info(log, fmt, ...) atlib_log_writef(log, "INFO", __FILE__, __LINE__, fmt, __VA_ARGS__)
#  define atlib_dbglog_warn(log, fmt, ...) atlib_log_writef(log, "WARN", __FILE__, __LINE__, fmt, __VA_ARGS__)
#  define atlib_dbglog_error(log, fmt, ...) atlib_log_writef(log, "ERROR", __FILE__, __LINE__, fmt, __VA_ARGS__)
#  define atlib_dbglog_fatal(log, fmt, ...) atlib_log_writef(log, "FATAL", __FILE__, __LINE__, fmt, __VA_ARGS__)
#  define atlib_debug_loop(type, start, end, increment, statements) for(type __i = (type)(start); __i < (type)(end); __i += (type)(increment)) { statements; }
#else
#  define atlib_dbglog_debug(...) if (0) {}
#  define atlib_dbglog_info(...) if (0) {}
#  define atlib_dbglog_warn(...) if (0) {}
#  define atlib_dbglog_error(...) if(0) {}
#  define atlib_dbglog_fatal(...) if(0) {}
#  define atlib_debug_loop(...) do {} while (0)
#endif

#endif

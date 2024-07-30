#ifndef __ATLIB_ERROR_H
#define __ATLIB_ERROR_H

#define ATLIB_ASSERT_ERRCODE 255

#include "Atlib/log.h"

#ifdef __DEBUG__
#  define atlib_compassert(exp) __atlib_assert_func((isize)(exp), #exp, __FILE__, __LINE__)
#  define atout __atlib_atout(__FILE__, __LINE__)
#  define aterr __atlib_aterr(__FILE__, __LINE__)
extern log_t * __atlib_atout(const char * file, i32 line) __attribute__ ((returns_nonnull));
extern log_t * __atlib_aterr(const char * file, i32 line) __attribute__ ((returns_nonnull));
#else
#  define atlib_compassert(exp) do {} while(0)
extern log_t * aterr;
extern log_t * atout;
#endif

#define atlib_assert(exp) __atlib_assert_func((isize)(exp), #exp, __FILE__, __LINE__)
extern void atlib_error_open(const char * out, const char * err);
extern void atlib_error_close(void);
extern void __atlib_assert_func(isize expval, const char * expression, const char * file, i32 line);

#endif

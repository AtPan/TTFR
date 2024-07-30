#ifndef __ATLIB_TYPES_H
#define __ATLIB_TYPES_H

#include <bits/types.h>
#include <bits/stdint-intn.h>
#include <bits/stdint-uintn.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef __u8_DEFINED__ 
#define __u8_DEFINED__ 
typedef uint8_t u8;
#endif
#ifndef __u16_DEFINED__ 
#define __u16_DEFINED__ 
typedef uint16_t u16;
#endif
#ifndef __u32_DEFINED__ 
#define __u32_DEFINED__ 
typedef uint32_t u32;
#endif
#ifndef __u64_DEFINED__ 
#define __u64_DEFINED__ 
typedef uint64_t u64;
#endif
#ifndef __usize_DEFINED__ 
#define __usize_DEFINED__ 
typedef __uintmax_t usize;
#endif

#ifndef __i8_DEFINED__ 
#define __i8_DEFINED__ 
typedef int8_t i8;
#endif
#ifndef __i16_DEFINED__ 
#define __i16_DEFINED__ 
typedef int16_t i16;
#endif
#ifndef __i32_DEFINED__ 
#define __i32_DEFINED__ 
typedef int32_t i32;
#endif
#ifndef __i64_DEFINED__ 
#define __i64_DEFINED__ 
typedef int64_t i64;
#endif
#ifndef __isize_DEFINED__ 
#define __isize_DEFINED__ 
typedef __intmax_t isize;
#endif

#ifndef __f32_DEFINED__
#define __f32_DEFINED__
typedef float f32;
#endif
#ifndef __f64_DEFINED__
#define __f64_DEFINED__
typedef double f64;
#endif
#ifndef __f128_DEFINED__
#define __f128_DEFINED__
typedef long double f128;
#endif

#endif

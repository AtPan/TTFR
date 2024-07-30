#ifndef __ATLIB_PROFILER_H
#define __ATLIB_PROFILER_H

#include <bits/types/clock_t.h>
#include <bits/types/FILE.h>
#include "types.h"
#include "bufwrite.h"

#ifndef __ATLIB_PROFILE_MAX_SEGMENTS
#define __ATLIB_PROFILE_MAX_SEGMENTS 100
#endif

#ifndef __ATLIB_PROFILE_MAX_CHILDREN
#define __ATLIB_PROFILE_MAX_CHILDREN 5
#endif

#ifndef __ATLIB_PROFILE_SEGMENT_NAME_LEN
#define __ATLIB_PROFILE_SEGMENT_NAME_LEN 50
#endif

typedef struct __segment {
    char name[__ATLIB_PROFILE_SEGMENT_NAME_LEN];
    clock_t start;
    clock_t length;
    struct __segment * parent;
} segment_t;

typedef bufwrite_t profile_bufwrite_t;

#ifndef __ATLIB_PROFILE_SEGMENT_DEFINITION_PROVIDED
extern segment_t __AtLib_profile_segments[__ATLIB_PROFILE_MAX_SEGMENTS];
extern segment_t * __AtLib_profile_head;
extern u16 __AtLib_num_segments;
#endif

extern profile_bufwrite_t * atlib_profile_open(bufwrite_t *__restrict, const char *__restrict);
extern profile_bufwrite_t * atlib_profile_fopen(bufwrite_t *__restrict, FILE *__restrict);
extern void atlib_profile_close(profile_bufwrite_t *);

extern void atlib_profile_start_segment(const char *);
extern void atlib_profile_start_segmentf(const char *, ...) __attribute__ ((format (printf, 1, 2)));
extern void atlib_profile_end_segment(void);
extern void atlib_profile_view(profile_bufwrite_t *);

#endif

#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#define __ATLIB_PROFILE_SEGMENT_DEFINITION_PROVIDED
#include "Atlib/profiler.h"

segment_t __AtLib_profile_segments[__ATLIB_PROFILE_MAX_SEGMENTS] = {0};
segment_t * __AtLib_profile_head = NULL;
u16 __AtLib_num_segments = 0;

void atlib_profile_start_segmentf(const char * fmt, ...) {
    char buf[__ATLIB_PROFILE_SEGMENT_NAME_LEN];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, __ATLIB_PROFILE_SEGMENT_NAME_LEN, fmt, ap);
    va_end(ap);

    atlib_profile_start_segment(buf);
}

void atlib_profile_start_segment(const char * segment_name) {
    segment_t * seg = NULL;

    /* If too many segments, quit */
    if(__AtLib_num_segments >= __ATLIB_PROFILE_MAX_SEGMENTS) return;

    /* Search for this segment if it already exists */
    for(usize i = 0; i < __AtLib_num_segments; i++) {
        if(!strncmp(__AtLib_profile_segments[i].name, segment_name, __ATLIB_PROFILE_SEGMENT_NAME_LEN)) {
            seg = &__AtLib_profile_segments[i];
            break;
        }
    }

    if(seg == NULL) { /* If no segment exists, make a new one */
        seg = &__AtLib_profile_segments[__AtLib_num_segments++];
        seg->parent = __AtLib_profile_head;
        __AtLib_profile_head = seg;
        strncpy(seg->name, segment_name, __ATLIB_PROFILE_SEGMENT_NAME_LEN);
    }

    seg->start = clock(); /* Start timer */
}

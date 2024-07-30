#include <stdio.h>
#include <time.h>
#include <string.h>
#include "Atlib/profiler.h"

void atlib_profile_end_segment() {
    clock_t end = clock();
    if(__AtLib_profile_head == NULL) return;
    __AtLib_profile_head->length += end - __AtLib_profile_head->start;
    __AtLib_profile_head = __AtLib_profile_head->parent;
}

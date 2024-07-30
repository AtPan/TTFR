#include <time.h>

#include "Atlib/bufwrite.h"
#include "Atlib/profiler.h"

void atlib_profile_view(profile_bufwrite_t * bw) {
    const segment_t * last_seg = &__AtLib_profile_segments[0];
    const segment_t * seg = last_seg;

    u32 indent = 1;
    usize local_head_time = 0;

    for(usize i = 0; i < __AtLib_num_segments; i++) {
        last_seg = seg;
        seg = &__AtLib_profile_segments[i];

        /* New local head */
        if(!seg->parent) {
            indent = 1;
            local_head_time = seg->length;
        }
        /* If this is a child of the last segment, increment the indentation */
        else if(seg->parent == last_seg) {
            indent++;
        }
        /* If this is an unrelated segment of the same head, pop out one indentation */
        else if(seg->parent != last_seg->parent) indent--;

        atlib_bufwrite_writef(bw, "%*c SEGMENT [%s] (%-5.2f%%): %f s\n", 4 * indent, '*', seg->name, 
                ((double)seg->length / local_head_time) * 100,
                ((double)seg->length / CLOCKS_PER_SEC));
    }
}

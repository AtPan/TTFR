#include <time.h>

#include "Atlib/profiler.h"

profile_bufwrite_t * atlib_profile_open(bufwrite_t * restrict bw, const char * restrict file_name) {
    struct tm * l;
    time_t t;

    if((bw = atlib_bufwrite_open(bw, file_name)) == NULL) return (profile_bufwrite_t *)NULL;
    t = time(NULL);
    l = localtime(&t);

    atlib_bufwrite_writef(bw, "OPENING PROFILER [%02d/%02d/%02d %02d:%02d:%02d]\n",
            l->tm_mon+1, l->tm_mday, l->tm_year+1900,
            l->tm_hour, l->tm_min, l->tm_sec);

    return (profile_bufwrite_t *)bw;
}

profile_bufwrite_t * atlib_profile_fopen(bufwrite_t * restrict bw, FILE * restrict file) {
    struct tm * l;
    time_t t;

    if((bw = atlib_bufwrite_fopen(bw, file)) == NULL) return (profile_bufwrite_t *)NULL;
    t = time(NULL);
    l = localtime(&t);

    atlib_bufwrite_writef(bw, "OPENING PROFILER [%02d/%02d/%02d %02d:%02d:%02d]\n",
            l->tm_mon+1, l->tm_mday, l->tm_year+1900,
            l->tm_hour, l->tm_min, l->tm_sec);

    return (profile_bufwrite_t *)bw;
}

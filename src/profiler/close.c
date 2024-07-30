#include <time.h>

#include "Atlib/profiler.h"

void atlib_profile_close(profile_bufwrite_t * bw) {
    time_t t = time(NULL);
    struct tm * l = localtime(&t);

    atlib_bufwrite_writef((bufwrite_t *)bw, "CLOSING PROFILER [%02d/%02d/%02d %02d:%02d:%02d]\n",
            l->tm_mon+1, l->tm_mday, l->tm_year+1900,
            l->tm_hour, l->tm_min, l->tm_sec);
    atlib_bufwrite_flush((bufwrite_t *)bw);
    atlib_bufwrite_close((bufwrite_t *)bw);
}

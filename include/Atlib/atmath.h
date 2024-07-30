#ifndef __ATLIB_MATH_H
#define __ATLIB_MATH_H

#include "types.h"

#define min(a, b) ((a) > (b) ? (b) : (a))
#define max(a, b) ((a) > (b) ? (a) : (b))

static inline u32 binc(u32 n, u32 k) {
    if(n < k) return 0;
    if(!(n | k | (n-k))) return 1;

    k = n - max(k, n-k);
    f64 val = (double)n / k;

    for(u32 i = 1; i < k; i++) {
        val *= ((double)(n - i)/(k - i));
    }

    return (u32)val;
}

#endif

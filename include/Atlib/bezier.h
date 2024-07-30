#ifndef __ATLIB_BEZIER_H
#define __ATLIB_BEZIER_H

#include "Atlib/types.h"

#ifndef ATLIB_NUM_CUBIC_BEZIER_POINTS
#define ATLIB_NUM_CUBIC_BEZIER_POINTS ((1 << 4) | (1 << 2))
#endif

typedef struct {
    f64 cubic_bezier_coefficients[(ATLIB_NUM_CUBIC_BEZIER_POINTS-2)*3];
} cubic_bezier_t;

typedef struct {
    i32 x;
    i32 y;
} Vec2;

extern cubic_bezier_t * atlib_initiate_cubic_bezier(cubic_bezier_t *);
extern Vec2 * atlib_cubic_bezier(const cubic_bezier_t *__restrict curve, Vec2 start, Vec2 end, Vec2 a, Vec2 b, Vec2 *__restrict points);

#endif

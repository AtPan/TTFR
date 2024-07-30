#include "Atlib/error.h"
#include "Atlib/bezier.h"

cubic_bezier_t * atlib_initiate_cubic_bezier(cubic_bezier_t * curve) {
    atlib_compassert(curve);

    f64 t = 0.0;

    for(u32 i = 0; i < ATLIB_NUM_CUBIC_BEZIER_POINTS - 2; i++) {
        t = (f64)(i+1) / (ATLIB_NUM_CUBIC_BEZIER_POINTS - 1);
        curve->cubic_bezier_coefficients[i*3] = t * t * t;
        curve->cubic_bezier_coefficients[(i*3)+1] = 3 * t * t;
        curve->cubic_bezier_coefficients[(i*3)+2] = 3 * t;
    }

    return curve;
}

static inline Vec2 vadd(Vec2 l, Vec2 r) {
    return (Vec2){.x = l.x + r.x, .y = l.y + r.y};
}

static inline Vec2 vsub(Vec2 l, Vec2 r) {
    return (Vec2){.x = l.x - r.x, .y = l.y - r.y};
}

static inline Vec2 vmul(Vec2 x, i32 c) {
    return (Vec2){.x = x.x * c, .y=x.y * c};
}

Vec2 * atlib_cubic_bezier(const cubic_bezier_t * restrict curve, Vec2 start, Vec2 end, Vec2 a, Vec2 b, Vec2 * restrict points) {
    atlib_compassert(curve);

    atlib_compassert(points);

    const f64 * coefs = curve->cubic_bezier_coefficients;

    const Vec2 _a = vadd(vsub(start, end), vmul(vsub(b, a), 3));
    const Vec2 _b = vsub(vadd(a, end), vmul(b, 2));
    const Vec2 _c = vsub(b, end);
    const Vec2 _d = end;

    points[0] = end;
    points[ATLIB_NUM_CUBIC_BEZIER_POINTS - 1] = start;

    for(u32 i = 1; i < ATLIB_NUM_CUBIC_BEZIER_POINTS - 1; i++) {
        const u32 k = (i-1) * 3;
        points[i] = (Vec2){
            .x = (coefs[k] * _a.x) + (coefs[k+1] * _b.x) + (coefs[k+2] * _c.x) + _d.x,
            .y = (coefs[k] * _a.y) + (coefs[k+1] * _b.y) + (coefs[k+2] * _c.y) + _d.y,
        };
    }

    return points;
}

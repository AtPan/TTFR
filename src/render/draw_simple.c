#include "Atlib/bezier.h"
#include "Atlib/error.h"
#include "Atlib/log.h"

#define __ATLIB_FONT_H
#include "AtFont/font_table.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

static inline double font_to_norm(int font_point, int upem) {
    return ((double)font_point + upem) / upem;
}

static inline int pt_to_px(int pt) {
    return pt << 1;
}

static inline int norm_to_px(double t, int px_max) {
    return t * px_max;
}

static inline int font_to_screen(int font_point, int pt, int upem, int offset) {
    return offset + norm_to_px(font_to_norm(font_point, upem), pt_to_px(pt));
}

void atlib_render_simple_glyf(SDL_Renderer * restrict render, const font_t * restrict font, const Glyf * restrict glyf, u16 pt, u32 xy) {
    atlib_compassert(render);

    atlib_compassert(font);
    atlib_compassert(font->glyf);

    atlib_compassert(glyf);

    const u16 x_off = xy >> 16;
    const u16 y_off = xy & 0x00ff;
    const u16 contours = glyf->header.numberOfContours;
    const u16 upem = font->head.unitsPerEm;
    const u8 * flags = glyf->simple.flags;

    Vec2 points[ATLIB_NUM_CUBIC_BEZIER_POINTS] = {0};

#ifdef __DEBUG__
    static i32 print = 0;
    i32 __start_off = 0;
    i32 __end_off = 0;

    if(print) goto __end_debug_print;
    for(u16 __i = 0; __i < contours; __i++) {
        i16 end_point = glyf->simple.endPtsOfContours[__i];
        i16 start_point = __i == 0 ? 0 : glyf->simple.endPtsOfContours[__i - 1] + 1;
        if(start_point == end_point) continue;
        atlib_dbglog_debug(atout, "Contour [%d]: [%d, %d]\n", __i, start_point, end_point);
        for(i16 i = start_point; i <= end_point; i++) {
            atlib_dbglog_debug(atout, "* Point [%02d]: (%4d,%5d) -> (%d,%3d) [%c]\n",
                i, glyf->simple.xCoordinates[i], glyf->simple.yCoordinates[i],
                font_to_screen(glyf->simple.xCoordinates[i], pt, upem, x_off),
                font_to_screen(glyf->simple.yCoordinates[i], pt, -upem, y_off),
                glyf->simple.flags[i] & ON_CURVE_POINT ? '*' : ' ');
        }
    }
__end_debug_print:
#endif

    for(u16 contour = 0; contour < contours; contour++) {
        i16 end_point = glyf->simple.endPtsOfContours[contour];
        i16 start_point = contour == 0 ? 0 : glyf->simple.endPtsOfContours[contour - 1] + 1;
        if(start_point == end_point) continue;

        // First, connect the contour's end points together. There are only so many
        // states that the end points can be in, which we must handle individually.

        // Potential contour end structures of spliced bezier curves:
        // | * ... * o o | => (1) First point is on curve, but last two are not
        // | o * ... * o | => (2) First and last points are off curve
        // | o o * ... * | => (3) First two points are off curve, but third and last are
        // | *   ...   * | => (4) First and last points are on curve

        /* Case (1) */
        if((flags[start_point] & ON_CURVE_POINT) && !(flags[end_point] & ON_CURVE_POINT)) {
            atlib_cubic_bezier(&font->curve,
                    (Vec2){.x = font_to_screen(glyf->simple.xCoordinates[end_point - 2], pt, upem, x_off),
                    .y = font_to_screen(glyf->simple.yCoordinates[end_point - 2], pt, -upem, y_off)},
                    (Vec2){.x = font_to_screen(glyf->simple.xCoordinates[start_point], pt, upem, x_off),
                    .y = font_to_screen(glyf->simple.yCoordinates[start_point], pt, -upem, y_off)},
                    (Vec2){.x = font_to_screen(glyf->simple.xCoordinates[end_point - 1], pt, upem, x_off),
                    .y = font_to_screen(glyf->simple.yCoordinates[end_point - 1], pt, -upem, y_off)},
                    (Vec2){.x = font_to_screen(glyf->simple.xCoordinates[end_point], pt, upem, x_off),
                    .y = font_to_screen(glyf->simple.yCoordinates[end_point], pt, -upem, y_off)},
                    points);
            for(u16 i = 0; i < ATLIB_NUM_CUBIC_BEZIER_POINTS - 1; i++) {
                SDL_RenderDrawLine(render, points[i].x, points[i].y, points[i+1].x, points[i+1].y);
            }
            end_point -= 2;
        }
        /* Case (2) */
        else if(!((flags[start_point] & ON_CURVE_POINT) || (flags[end_point] & ON_CURVE_POINT))) {
            atlib_cubic_bezier(&font->curve,
                    (Vec2){.x = font_to_screen(glyf->simple.xCoordinates[end_point - 1], pt, upem, x_off),
                    .y = font_to_screen(glyf->simple.yCoordinates[end_point - 1], pt, -upem, y_off)},
                    (Vec2){.x = font_to_screen(glyf->simple.xCoordinates[start_point + 1], pt, upem, x_off),
                    .y = font_to_screen(glyf->simple.yCoordinates[start_point + 1], pt, -upem, y_off)}, 
                    (Vec2){.x = font_to_screen(glyf->simple.xCoordinates[end_point], pt, upem, x_off),
                    .y = font_to_screen(glyf->simple.yCoordinates[end_point], pt, -upem, y_off)}, 
                    (Vec2){.x = font_to_screen(glyf->simple.xCoordinates[start_point], pt, upem, x_off),
                    .y = font_to_screen(glyf->simple.yCoordinates[start_point], pt, -upem, y_off)}, 
                    points);
            for(u16 i = 0; i < ATLIB_NUM_CUBIC_BEZIER_POINTS - 1; i++) {
                SDL_RenderDrawLine(render, points[i].x, points[i].y, points[i+1].x, points[i+1].y);
            }
            start_point++;
            end_point--;
        }
        /* Case (3) */
        else if(!(flags[start_point] & ON_CURVE_POINT) && (flags[end_point] & ON_CURVE_POINT)) {
            atlib_cubic_bezier(&font->curve,
                    (Vec2){.x = font_to_screen(glyf->simple.xCoordinates[end_point], pt, upem, x_off),
                    .y = font_to_screen(glyf->simple.yCoordinates[end_point], pt, -upem, y_off)}, 
                    (Vec2){.x = font_to_screen(glyf->simple.xCoordinates[start_point + 2], pt, upem, x_off),
                    .y = font_to_screen(glyf->simple.yCoordinates[start_point + 2], pt, -upem, y_off)},
                    (Vec2){.x = font_to_screen(glyf->simple.xCoordinates[start_point], pt, upem, x_off),
                    .y = font_to_screen(glyf->simple.yCoordinates[start_point], pt, -upem, y_off)},
                    (Vec2){.x = font_to_screen(glyf->simple.xCoordinates[start_point + 1], pt, upem, x_off),
                    .y = font_to_screen(glyf->simple.yCoordinates[start_point + 1], pt, -upem, y_off)},
                    points);
            for(u16 i = 0; i < ATLIB_NUM_CUBIC_BEZIER_POINTS - 1; i++) {
                SDL_RenderDrawLine(render, points[i].x, points[i].y, points[i+1].x, points[i+1].y);
            }
            start_point += 2;
        }
        /* Case (4) */
        else {
            SDL_RenderDrawLine(render,
                    font_to_screen(glyf->simple.xCoordinates[start_point], pt, upem, x_off),
                    font_to_screen(glyf->simple.yCoordinates[start_point], pt, -upem, y_off),
                    font_to_screen(glyf->simple.xCoordinates[end_point], pt, upem, x_off),
                    font_to_screen(glyf->simple.yCoordinates[end_point], pt, -upem, y_off));
        }


        i16 i = start_point;
#ifdef __DEBUG__
        if(!print) {
            i16 __i = i;
            if((flags[start_point] & ON_CURVE_POINT) && !(flags[end_point] & ON_CURVE_POINT)) {
                __end_off = 2;
                __start_off = 0;
                atlib_dbglog_debug(atout, "%s",  "Case (1):\n");
            }
            else if(!((flags[start_point] & ON_CURVE_POINT) || (flags[end_point] & ON_CURVE_POINT))) {
                __end_off = 1;
                __start_off = 0;
                atlib_dbglog_debug(atout, "%s", "Case (2):\n");
            }
            else if(!(flags[start_point] & ON_CURVE_POINT) && (flags[end_point] & ON_CURVE_POINT)) {
                __end_off = 0;
                __start_off = 1;
                atlib_dbglog_debug(atout, "%s", "Case (3):\n");
            }
            else {
                __end_off = __start_off = 0;
                atlib_dbglog_debug(atout, "%s", "Case (4):\n");
            }

            atlib_dbglog_debug(atout, "\tFrom (%d, %d) -> (%d, %d)\n",
                font_to_screen(glyf->simple.xCoordinates[end_point-__end_off], pt, upem, x_off),
                font_to_screen(glyf->simple.yCoordinates[end_point-__end_off], pt, -upem, y_off),
                font_to_screen(glyf->simple.xCoordinates[start_point+__start_off], pt, upem, x_off),
                font_to_screen(glyf->simple.yCoordinates[start_point+__start_off], pt, -upem, y_off));
            while(i < end_point) {
                if(flags[i + 1] & ON_CURVE_POINT) {
                    atlib_dbglog_debug(atout, "Drawing Straight Segment [%2d] -> [%2d]: (%3d,%4d) -> (%3d,%4d)\n", 
                            i, i+1, font_to_screen(glyf->simple.xCoordinates[i], pt, upem, x_off),
                            font_to_screen(glyf->simple.yCoordinates[i], pt, -upem, y_off),
                            font_to_screen(glyf->simple.xCoordinates[i + 1], pt, upem, x_off),
                            font_to_screen(glyf->simple.yCoordinates[i + 3], pt, -upem, y_off));
                    i++;
                }
                else {
                    atlib_dbglog_debug(atout, "Drawing Curved Segment [%2d] -> [%2d]: (%3d,%4d) -> (%3d,%4d)\n",
                            i, i+3, font_to_screen(glyf->simple.xCoordinates[i], pt, upem, x_off),
                            font_to_screen(glyf->simple.yCoordinates[i], pt, -upem, y_off),
                            font_to_screen(glyf->simple.xCoordinates[i + 3], pt, upem, x_off),
                            font_to_screen(glyf->simple.yCoordinates[i + 3], pt, -upem, y_off));

                    atlib_cubic_bezier(&font->curve,
                            (Vec2){.x = font_to_screen(glyf->simple.xCoordinates[i], pt, upem, x_off),
                            .y = font_to_screen(glyf->simple.yCoordinates[i], pt, -upem, y_off)},
                            (Vec2){.x = font_to_screen(glyf->simple.xCoordinates[i + 3], pt, upem, x_off),
                            .y = font_to_screen(glyf->simple.yCoordinates[i + 3], pt, -upem, y_off)},
                            (Vec2){.x = font_to_screen(glyf->simple.xCoordinates[i + 1], pt, upem, x_off),
                            .y = font_to_screen(glyf->simple.yCoordinates[i + 1], pt, -upem, y_off)},
                            (Vec2){.x = font_to_screen(glyf->simple.xCoordinates[i + 2], pt, upem, x_off),
                            .y = font_to_screen(glyf->simple.yCoordinates[i + 2], pt, -upem, y_off)},
                            points);
                    for(u16 k = 0; k < ATLIB_NUM_CUBIC_BEZIER_POINTS - 1; k++) {
                        atlib_dbglog_debug(atout, "\tCurve Segment [%2d] -> [%2d]: (%3d,%4d) -> (%3d,%4d)\n",
                                k, k+1, points[k].x, points[k].y, points[k+1].x, points[k+1].y);
                    }
                    i += 3;
                }
            }
            i = __i;
        }
#endif

        while(i < end_point) {
            if(flags[i + 1] & ON_CURVE_POINT) {
                SDL_RenderDrawLine(render, 
                        font_to_screen(glyf->simple.xCoordinates[i], pt, upem, x_off),
                        font_to_screen(glyf->simple.yCoordinates[i], pt, -upem, y_off),
                        font_to_screen(glyf->simple.xCoordinates[i + 1], pt, upem, x_off),
                        font_to_screen(glyf->simple.yCoordinates[i + 1], pt, -upem, y_off));
                i++;
            }
            else {
                atlib_cubic_bezier(&font->curve,
                        (Vec2){.x = font_to_screen(glyf->simple.xCoordinates[i], pt, upem, x_off),
                        .y = font_to_screen(glyf->simple.yCoordinates[i], pt, -upem, y_off)},
                        (Vec2){.x = font_to_screen(glyf->simple.xCoordinates[i + 3], pt, upem, x_off),
                        .y = font_to_screen(glyf->simple.yCoordinates[i + 3], pt, -upem, y_off)},
                        (Vec2){.x = font_to_screen(glyf->simple.xCoordinates[i + 1], pt, upem, x_off),
                        .y = font_to_screen(glyf->simple.yCoordinates[i + 1], pt, -upem, y_off)},
                        (Vec2){.x = font_to_screen(glyf->simple.xCoordinates[i + 2], pt, upem, x_off),
                        .y = font_to_screen(glyf->simple.yCoordinates[i + 2], pt, -upem, y_off)},
                        points);
                for(u16 k = 0; k < ATLIB_NUM_CUBIC_BEZIER_POINTS - 1; k++) {
                    SDL_RenderDrawLine(render, points[k].x, points[k].y, points[k+1].x, points[k+1].y);
                }
                i += 3;
            }
        }

    }

#ifdef __DEBUG__
    if(!print) print++;
#endif
}

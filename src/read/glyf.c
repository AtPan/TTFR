#include "Atlib/bufread.h"
#include "Atlib/error.h"

#define __ATLIB_FONT_H
#include "AtFont/font_internal.h"
#include "AtFont/font_table.h"

#include <stdlib.h>
#include <string.h>

static Glyf_optional_t read_simple_glyf(Glyf g, bufread_t * br);
static Glyf_optional_t read_complex_glyf(Glyf g, const Maxp * maxp, bufread_t * br);

static inline double read_f2dot14(bufread_t * br) {
    register i16 val = atlib_bufread_read_i16(br);
    return (val >> 14) + ((double)(val & 0x3fff) / 0x4000);
}

Glyf_optional_t * atlib_font_read_glyf(
        const TableDirectory * restrict dir,
        const Maxp * restrict maxp,
        const Loca * restrict loca,
        bufread_t * restrict br)
{
    Glyf_optional_t * glyfs = NULL;
    const TableRecord * rec;
    Glyf g;
    usize offset;

    atlib_compassert(dir);
    atlib_compassert(dir->recs);

    atlib_compassert(maxp);

    atlib_compassert(loca);
    atlib_compassert(loca->offset);

    atlib_compassert(br);
    atlib_compassert(br->fh);

    if((rec = __find_tag_record(dir, "glyf")) == NULL
     || (glyfs = malloc(sizeof(Glyf_optional_t) * maxp->numGlyphs)) == NULL) goto err;
    memset(glyfs, 0, sizeof(Glyf_optional_t) * maxp->numGlyphs);
    offset = rec->offset;

    atlib_bufread_seek(br, offset);

    for(u16 i = 0; i < maxp->numGlyphs; i++) {
        for(; i + 1 < maxp->numGlyphs && loca->offset[i] == loca->offset[i + 1]; i++) {
            glyfs[i] = Glyf_optional_none();
        }
        
        atlib_bufread_seek(br, offset + loca->offset[i]);

        g.header.numberOfContours = atlib_bufread_read_i16(br);
        g.header.xMin = atlib_bufread_read_i16(br);
        g.header.yMin = atlib_bufread_read_i16(br);
        g.header.xMax = atlib_bufread_read_i16(br);
        g.header.yMax = atlib_bufread_read_i16(br);

        if(g.header.numberOfContours == 0) glyfs[i] = Glyf_optional_none();
        else if(g.header.numberOfContours < 0) glyfs[i] = read_complex_glyf(g, maxp, br);
        else glyfs[i] = read_simple_glyf(g, br);

        if(g.header.numberOfContours != 0 && glyfs[i].is_some == 0) goto read;
    }

    if(!(atlib_bufread_eof(br) || atlib_bufread_err(br))) goto ret;

read:
    free(glyfs);
err:
    glyfs = NULL;
ret:
    return glyfs;
}

void atlib_font_free_glyf(const Maxp * restrict maxp, Glyf_optional_t * restrict glyfs) {
    atlib_compassert(maxp);
    atlib_compassert(glyfs);

    for(u16 i = 0; i < maxp->numGlyphs; i++) {
        if(!glyfs[i].is_some) continue;
        Glyf g = Glyf_optional_unwrap(glyfs[i]);
        if(g.header.numberOfContours > 0) {
            free(g.simple.endPtsOfContours);
            free(g.simple.flags);
        }
        else if(g.header.numberOfContours < 0) {
            free(g.complex.components);
        }
    }
    free(glyfs);
}

static Glyf_optional_t read_simple_glyf(Glyf g, bufread_t * br) {
    i16 numConts = g.header.numberOfContours;
    u16 points;
    void * buf;
    register u8 f;

    if((g.simple.endPtsOfContours = malloc(sizeof(u16) * numConts)) == NULL) goto err;
    for(i16 i = 0; i < numConts; i++) {
        g.simple.endPtsOfContours[i] = atlib_bufread_read_u16(br);
    }
    points = g.simple.endPtsOfContours[numConts - 1] + 1;
    if((buf = malloc(5 * points)) == NULL) goto numConts;
    memset(buf, 0, 5 * points);
    g.simple.flags = (u8 *)buf;
    g.simple.xCoordinates = (i16 *)((usize)buf + points);
    g.simple.yCoordinates = (i16 *)((usize)buf + (3 * points));
    g.simple.instructionLength = atlib_bufread_read_u16(br);

    atlib_bufread_skip(br, g.simple.instructionLength);

    for(u16 i = 0; i < points; i++) {
        f = g.simple.flags[i] = atlib_bufread_read_u8(br);
        if(f & SIMPLE_FLAG_RESERVED) goto buf;
        if(f & REPEAT_FLAG) {
            u8 repeat = atlib_bufread_read_u8(br);
            for(; repeat > 0; repeat--) {
                g.simple.flags[++i] = f;
            }
        }
    }

    f = g.simple.flags[0];
    if(f & X_SHORT_VECTOR) {
        if(f & X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) g.simple.xCoordinates[0] = atlib_bufread_read_u8(br);
        else g.simple.xCoordinates[0] = (-1) * (i16)atlib_bufread_read_u8(br);
    }
    else {
        g.simple.xCoordinates[0] = atlib_bufread_read_i16(br);
    }
    for(u16 i = 1; i < points; i++) {
        f = g.simple.flags[i];
        g.simple.xCoordinates[i] = g.simple.xCoordinates[i - 1];
        if(f & X_SHORT_VECTOR) {
            if(f & X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) g.simple.xCoordinates[i] += atlib_bufread_read_u8(br);
            else g.simple.xCoordinates[i] -= atlib_bufread_read_u8(br);
        }
        else if(!(f & X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR)) {
            g.simple.xCoordinates[i] += atlib_bufread_read_i16(br);
        }
    }

    f = g.simple.flags[0];
    if(f & Y_SHORT_VECTOR) {
        if(f & Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR) g.simple.yCoordinates[0] = atlib_bufread_read_u8(br);
        else g.simple.yCoordinates[0] = (-1) * (i16)atlib_bufread_read_u8(br);
    }
    else {
        g.simple.yCoordinates[0] = atlib_bufread_read_i16(br);
    }
    for(u16 i = 1; i < points; i++) {
        f = g.simple.flags[i];
        g.simple.yCoordinates[i] = g.simple.yCoordinates[i - 1];
        if(f & Y_SHORT_VECTOR) {
            if(f & Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR) g.simple.yCoordinates[i] += atlib_bufread_read_u8(br);
            else g.simple.yCoordinates[i] -= atlib_bufread_read_u8(br);
        }
        else if(!(f & Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR)){
            g.simple.yCoordinates[i] += atlib_bufread_read_i16(br);
        }
    }

    if(!(atlib_bufread_err(br) || atlib_bufread_eof(br))) goto ret;
buf:
    free(buf);
numConts:
    free(g.simple.endPtsOfContours);
err:
    return Glyf_optional_none();
ret:
    return Glyf_optional_some(g);
}

static Glyf_optional_t read_complex_glyf(Glyf g, const Maxp * restrict maxp, bufread_t * restrict br) {
    register u16 f = MORE_COMPONENTS;
    u16 i;
    ComplexGlyfComponent * components, * tmp;

    if((components = malloc(sizeof(ComplexGlyfComponent) * maxp->maxComponentElements)) == NULL) goto err;
    for(i = 0; f & MORE_COMPONENTS; i++) {
        f = components[i].flags = atlib_bufread_read_u16(br);
        components[i].glyphIndex = atlib_bufread_read_u16(br);

        if(f & ARG_1_AND_2_ARE_WORDS) {
            components[i].argument1 = atlib_bufread_read_i16(br);
            components[i].argument2 = atlib_bufread_read_i16(br);
        }
        else {
            components[i].argument1 = atlib_bufread_read_u16(br);
            components[i].argument2 = (u8)components[i].argument1;
            components[i].argument1 >>= 8;
        }

        if(f & WE_HAVE_A_SCALE) {
            components[i].xscale = components[i].yscale = read_f2dot14(br);
        }
        else if(f & WE_HAVE_AN_X_AND_Y_SCALE) {
            components[i].xscale = read_f2dot14(br);
            components[i].yscale = read_f2dot14(br);
        }
        else if(f & WE_HAVE_A_TWO_BY_TWO) {
            components[i].xscale = read_f2dot14(br);
            components[i].scale_1 = read_f2dot14(br);
            components[i].scale_2 = read_f2dot14(br);
            components[i].yscale = read_f2dot14(br);
        }
    }
    if(f & WE_HAVE_INSTRUCTIONS) {
        atlib_bufread_seek(br, atlib_bufread_read_u16(br));
    }
    if((tmp = realloc(components, sizeof(ComplexGlyfComponent) * i)) == NULL) goto comp;
    g.complex.components = tmp;

    if(!(atlib_bufread_eof(br) || atlib_bufread_err(br))) goto ret;
comp:
    free(components);
err:
    return Glyf_optional_none();
ret:
    return Glyf_optional_some(g);
}


#if !defined(__ATLIB_FONT_H) && !defined(__DEBUG__)
#error "ATLIB ERROR: Do not use \"font_internal.h\" directly; Use \"AtFont.h\" instead"
#endif

#ifndef __ATLIB_FONT_INTERNAL_H
#define __ATLIB_FONT_INTERNAL_H

#include <SDL2/SDL_render.h>
#include "Atlib/bufread.h"
#include "AtFont/font_table.h"

TableDirectory * atlib_font_read_directory(TableDirectory *, bufread_t *);
void atlib_font_free_directory(TableDirectory *);

Cmap * atlib_font_read_cmap(const TableDirectory *, Cmap *, bufread_t *);
void atlib_font_free_cmap(Cmap *);

Loca * atlib_font_read_loca(const TableDirectory *, const Head *, const Maxp *, Loca *, bufread_t *);
void atlib_font_free_loca(Loca *);

Hmtx * atlib_font_read_hmtx(const TableDirectory *, const Maxp *, const Hhea *, Hmtx *, bufread_t *);
void atlib_font_free_hmtx(Hmtx *);

Glyf_optional_t * atlib_font_read_glyf(const TableDirectory *, const Maxp *, const Loca *, bufread_t *);
void atlib_font_free_glyf(const Maxp *, Glyf_optional_t *);

Head * atlib_font_read_head(const TableDirectory *, Head *, bufread_t *);
Maxp * atlib_font_read_maxp(const TableDirectory *, Maxp *, bufread_t *);
Hhea * atlib_font_read_hhea(const TableDirectory *, Hhea *, bufread_t *);

Glyf_optional_t atlib_font_to_glyf(const font_t *, char);
void draw_simple_glyf(SDL_Renderer *, const Head *, const Glyf *, u16, u16, u16);

extern void atlib_render_simple_glyf(SDL_Renderer *__restrict render, const font_t *__restrict font, const Glyf *__restrict glyf, u16 pt, u32 xy);

static inline TableRecord * __find_tag_record(const TableDirectory * restrict dir, const char tag[5]) {
    for(usize i = 0; i < dir->numTables; i++) {
        for(usize j = 0; j < 4; j++) {
            if(dir->recs[i].tableTag[j] != tag[j]) break;
            if(j == 3) return &dir->recs[i];
        }
    }
    return NULL;
}

#endif

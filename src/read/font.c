#include "Atlib/error.h"
#include "Atlib/bezier.h"

#include "AtFont.h"
#include "AtFont/font_table.h"
#include "AtFont/font_internal.h"

#include <stdlib.h>

const usize atlib_font_size() {
    return sizeof(font_t);
}

font_t * atlib_font_open(font_t * restrict font, bufread_t * restrict br) {
    atlib_compassert(font);

    atlib_compassert(br);
    atlib_compassert(br->fh);
    
    if(atlib_font_read_directory(&font->dir, br) == NULL) goto err;
    if(atlib_font_read_head(&font->dir, &font->head, br) == NULL) goto dir;
    if(atlib_font_read_hhea(&font->dir, &font->hhea, br) == NULL) goto dir;
    if(atlib_font_read_maxp(&font->dir, &font->maxp, br) == NULL) goto dir;
    if(atlib_font_read_cmap(&font->dir, &font->cmap, br) == NULL) goto dir;
    if(atlib_font_read_loca(&font->dir, &font->head, &font->maxp, &font->loca, br) == NULL) goto cmap;
    if(atlib_font_read_hmtx(&font->dir, &font->maxp, &font->hhea, &font->hmtx, br) == NULL) goto loca;
    if((font->glyf = atlib_font_read_glyf(&font->dir, &font->maxp, &font->loca, br)) == NULL) goto htmx;

    atlib_initiate_cubic_bezier(&font->curve);

    goto ret;
htmx:
    atlib_font_free_hmtx(&font->hmtx);
loca:
    atlib_font_free_loca(&font->loca);
cmap:
    atlib_font_free_cmap(&font->cmap);
dir:
    atlib_font_free_directory(&font->dir);
err:
    font = NULL;
ret:
    return font;
}

void atlib_font_free(font_t * font) {
    atlib_compassert(font);

    atlib_font_free_directory(&font->dir);
    atlib_font_free_cmap(&font->cmap);
    atlib_font_free_loca(&font->loca);
    atlib_font_free_hmtx(&font->hmtx);
    atlib_font_free_glyf(&font->maxp, font->glyf);
}

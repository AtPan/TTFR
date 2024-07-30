#include "Atlib/bufread.h"
#include "Atlib/error.h"

#define __ATLIB_FONT_H
#include "AtFont/font_internal.h"

Hmtx * atlib_font_read_hmtx(
        const TableDirectory * restrict dir,
        const Maxp * restrict maxp,
        const Hhea * restrict hhea,
        Hmtx * restrict hmtx,
        bufread_t * restrict br) 
{
    atlib_compassert(dir);
    atlib_compassert(dir->recs);

    atlib_compassert(maxp);

    atlib_compassert(hhea);

    atlib_compassert(hmtx);

    atlib_compassert(br);
    atlib_compassert(br->fh);

    TableRecord * rec = __find_tag_record(dir, "hmtx");
    atlib_bufread_seek(br, rec->offset);

    i16 diff = maxp->numGlyphs - hhea->numberOfHMetrics;


    if((hmtx->hMetrics = malloc(sizeof(LongHorMetric) * hhea->numberOfHMetrics)) == NULL) goto hm;
    if(diff > 0) {
        if((hmtx->leftSideBearings = malloc(sizeof(i16) * diff)) == NULL) goto lsb;
    }
    else hmtx->leftSideBearings = NULL;

    for(u16 i = 0; i < hhea->numberOfHMetrics; i++) {
        hmtx->hMetrics[i].advanceWidth = atlib_bufread_read_u16(br);
        hmtx->hMetrics[i].lsb = atlib_bufread_read_i16(br);
    }
    for(u16 i = 0; i < diff; i++) {
        hmtx->leftSideBearings[i] = atlib_bufread_read_i16(br);
    }

    if(!(atlib_bufread_eof(br) || atlib_bufread_err(br))) goto ret;
    if(hmtx->leftSideBearings) free(hmtx->leftSideBearings);
lsb:
    free(hmtx->hMetrics);
hm:
    hmtx = NULL;
ret:
    return hmtx;
}

void atlib_font_free_hmtx(Hmtx * hmtx) {
    if(hmtx->leftSideBearings) free(hmtx->leftSideBearings);
    free(hmtx->hMetrics);
}

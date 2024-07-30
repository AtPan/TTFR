#include "Atlib/error.h"

#define __ATLIB_FONT_H
#include "AtFont/font_internal.h"

Glyf_optional_t atlib_font_to_glyf(const font_t * restrict font, char c) {
    atlib_compassert(font);
    atlib_compassert(font->glyf);

    // Ascii into Unicode Point -> Cmap table into Glyf Index -> Loca table into offset -> Glyf
    const Cmap cmap = font->cmap;
    const u16 segs = cmap.subtable.segCountX2 / 2;
    const u8 data = (u8)c;
    u16 i;

    for(i = 0; i < segs && cmap.subtable.endCodes[i] < data; i++);
    if(i == segs || cmap.subtable.startCodes[i] > data) goto err;

    // Use idDelta and idRangeOffset to map data into a glyph index
    if(cmap.subtable.idRangeOffsets[i]) {
        // Use glyfIdArray
        return font->glyf[cmap.subtable.glyfIdArrays[(cmap.subtable.idRangeOffsets[i]/2) + (data - cmap.subtable.startCodes[i]) + segs - i - 1]];
    }
    else return font->glyf[cmap.subtable.idDeltas[i] + data];

err:
    return font->glyf[0];
}

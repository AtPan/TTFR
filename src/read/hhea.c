#include "Atlib/bufread.h"
#include "Atlib/error.h"

#define __ATLIB_FONT_H
#include "AtFont/font_internal.h"

Hhea * atlib_font_read_hhea(const TableDirectory * restrict dir, Hhea * restrict hhea, bufread_t * restrict br) {
    atlib_compassert(dir);
    atlib_compassert(dir->recs);

    atlib_compassert(hhea);

    atlib_compassert(br);
    atlib_compassert(br->fh);

    const TableRecord * rec = __find_tag_record(dir, "hhea");
    atlib_bufread_seek(br, rec->offset);
    
    if((hhea->majorVersion = atlib_bufread_read_u16(br)) != 0x0001) return NULL;
    if((hhea->minorVersion = atlib_bufread_read_u16(br)) != 0x0000) return NULL;
    hhea->ascender = atlib_bufread_read_i16(br);
    hhea->descender = atlib_bufread_read_i16(br);
    hhea->lineGap = atlib_bufread_read_i16(br);
    hhea->advanceWidthMax = atlib_bufread_read_u16(br);
    hhea->minLeftSideBearing = atlib_bufread_read_i16(br);
    hhea->minRightSideBearing = atlib_bufread_read_i16(br);
    hhea->xMaxExtent = atlib_bufread_read_i16(br);
    hhea->caretSlopRise = atlib_bufread_read_i16(br);
    hhea->caretSlopRun = atlib_bufread_read_i16(br);
    hhea->caretOffset = atlib_bufread_read_i16(br);
    for(usize i = 0; i < 4; i++) {
        /* 4 reserved i16 fields set to 0, +1 field set to 0 for current format. */
        if(atlib_bufread_read_i16(br) != 0) return NULL;
    }
    if((hhea->metricDataFormat = atlib_bufread_read_i16(br)) != 0x0000) return NULL;
    hhea->numberOfHMetrics = atlib_bufread_read_u16(br);

    if(atlib_bufread_err(br) || atlib_bufread_eof(br)) return NULL;
    return hhea;
}

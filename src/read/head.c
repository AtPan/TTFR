#include "Atlib/error.h"
#include "Atlib/bufread.h"

#define __ATLIB_FONT_H
#include "AtFont/font_internal.h"

Head * atlib_font_read_head(const TableDirectory * restrict dir, Head * restrict head, bufread_t * restrict br) {
    atlib_compassert(dir);
    atlib_compassert(dir->recs);

    atlib_compassert(head);

    atlib_compassert(br);
    atlib_compassert(br->fh);

    const TableRecord * rec;
    if((rec = __find_tag_record(dir, "head")) == NULL) return NULL;
    atlib_bufread_seek(br, rec->offset);

    if((head->majorVersion = atlib_bufread_read_u16(br)) != 0x0001) return NULL;
    if((head->minorVersion = atlib_bufread_read_u16(br)) != 0x0000) return NULL;
    head->fontRevision = atlib_bufread_read_i32(br);
    head->checksumAdjustment = atlib_bufread_read_u32(br);
    if((head->magicNumber = atlib_bufread_read_u32(br)) != 0x5f0f3cf5) return NULL;
    if((head->flags = atlib_bufread_read_u16(br)) & 0x8000) return NULL;
    head->unitsPerEm = atlib_bufread_read_u16(br);
    head->created = atlib_bufread_read_i64(br);
    head->modified = atlib_bufread_read_i64(br);
    head->xMin = atlib_bufread_read_i16(br);
    head->yMin = atlib_bufread_read_i16(br);
    head->xMax = atlib_bufread_read_i16(br);
    head->yMax = atlib_bufread_read_i16(br);
    head->macStyle = atlib_bufread_read_u16(br);
    head->lowestRecPPEM = atlib_bufread_read_u16(br);
    head->fontDirectionHint = atlib_bufread_read_i16(br);
    head->indexToLocFormat = atlib_bufread_read_i16(br);
    head->glyphDataFormat = atlib_bufread_read_i16(br);

    return (head->glyphDataFormat || atlib_bufread_err(br) || atlib_bufread_eof(br)) ? NULL : head;
}

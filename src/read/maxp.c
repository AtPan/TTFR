#include "Atlib/error.h"

#define __ATLIB_FONT_H
#include "AtFont/font_internal.h"

Maxp * atlib_font_read_maxp(const TableDirectory * restrict dir, Maxp * restrict maxp, bufread_t * restrict br) {
    atlib_compassert(dir);
    atlib_compassert(dir->recs);

    atlib_compassert(maxp);

    atlib_compassert(br);
    atlib_compassert(br->fh);

    const TableRecord * rec;
    if((rec = __find_tag_record(dir, "maxp")) == NULL) return NULL;
    atlib_bufread_seek(br, rec->offset);
    if((maxp->version = atlib_bufread_read_u32(br)) != 0x00010000) return NULL;
    maxp->numGlyphs = atlib_bufread_read_u16(br);
    maxp->maxPoints = atlib_bufread_read_u16(br);
    maxp->maxContours = atlib_bufread_read_u16(br);
    maxp->maxCompositPoints = atlib_bufread_read_u16(br);
    maxp->maxCompositContours = atlib_bufread_read_u16(br);
    maxp->maxZones = atlib_bufread_read_u16(br);
    maxp->maxTwilightPoints = atlib_bufread_read_u16(br);
    maxp->maxStorage = atlib_bufread_read_u16(br);
    maxp->maxFunctionDefs = atlib_bufread_read_u16(br);
    maxp->maxInstructionDefs = atlib_bufread_read_u16(br);
    maxp->maxStackElements = atlib_bufread_read_u16(br);
    maxp->maxSizeOfInstructions = atlib_bufread_read_u16(br);
    maxp->maxComponentElements = atlib_bufread_read_u16(br);
    maxp->maxComponentDepth = atlib_bufread_read_u16(br);

    if(atlib_bufread_eof(br) || atlib_bufread_err(br)) return NULL;
    return maxp;
}

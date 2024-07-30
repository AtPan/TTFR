#include "Atlib/bufread.h"
#include "Atlib/error.h"

#define __ATLIB_FONT_H
#include "AtFont/font_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Cmap * atlib_font_read_cmap(const TableDirectory * restrict dir, Cmap * restrict cmap, bufread_t * br) {
    atlib_compassert(dir);
    atlib_compassert(dir->recs);

    atlib_compassert(cmap);

    atlib_compassert(br);
    atlib_compassert(br->fh);
    
    /* Local Variable Declerations */
    TableRecord * cmap_rec;
    CmapEncodingRecord * recs;
    u16 * buf;
    u32 cmap_table_offset;
    u16 table_idx;

    /* Find, and seek to, the start of the cmap table */
    if((cmap_rec = __find_tag_record(dir, "cmap")) == NULL) goto err;
    cmap_table_offset = cmap_rec->offset;
    atlib_bufread_seek(br, cmap_table_offset);

    /* Read basic cmap header */
    if((cmap->version = atlib_bufread_read_u16(br))) goto err;
    cmap->numTables = atlib_bufread_read_u16(br);

    /* Allocate memory for cmap encoding records */
    if((recs = malloc(sizeof(CmapEncodingRecord) * cmap->numTables)) == NULL) goto err;

    for(table_idx = 0; table_idx < cmap->numTables; table_idx++) {
        recs[table_idx].platformID = atlib_bufread_read_u16(br);
        recs[table_idx].encodingID = atlib_bufread_read_u16(br);
        recs[table_idx].subtableOffset = atlib_bufread_read_u32(br);

        /* If we find a Unicode 2.0+ BMP supported table, quit from the loop early */
        if(recs[table_idx].platformID == 0 && recs[table_idx].encodingID == 3) break;
    }
    cmap->encodingRecords = recs;

    /* If we found no supported encoding record, quit */
    if(table_idx == cmap->numTables) goto encrec;

    /* Skip to the subtable pointed to by the encoding record */
    atlib_bufread_seek(br, cmap_table_offset + recs[table_idx].subtableOffset);

    /* If the subtable is not a supported version, quit */
    if((cmap->subtable.format = atlib_bufread_read_u16(br)) != 4) goto encrec;
    cmap->subtable.length = atlib_bufread_read_u16(br);
    cmap->subtable.language = atlib_bufread_read_u16(br);
    cmap->subtable.segCountX2 = atlib_bufread_read_u16(br);
    cmap->subtable.searchRange = atlib_bufread_read_u16(br);
    cmap->subtable.entrySelector = atlib_bufread_read_u16(br);
    cmap->subtable.rangeShift = atlib_bufread_read_u16(br);

    /* Allocate memory for endCode, startCode, idDelta, idRangeOffset, and glyphIdArray */
    if((buf = malloc(cmap->subtable.length - 16)) == NULL) goto encrec;
    memset(buf, 0, cmap->subtable.length - 16);
    cmap->subtable.endCodes = buf;
    cmap->subtable.startCodes = (u16 *)((usize)buf + cmap->subtable.segCountX2);
    cmap->subtable.idDeltas = (i16 *)((usize)buf + (2 * cmap->subtable.segCountX2));
    cmap->subtable.idRangeOffsets = (u16 *)((usize)buf + (3 * cmap->subtable.segCountX2));
    cmap->subtable.glyfIdArrays = (u16 *)((usize)buf + (4 * cmap->subtable.segCountX2));
    const u16 segs = cmap->subtable.segCountX2 >> 1;

    for(u16 i = 0; i < segs; i++) {
        cmap->subtable.endCodes[i] = atlib_bufread_read_u16(br);
    }
    if(atlib_bufread_read_u16(br)) goto arrays;
    for(u16 i = 0; i < segs; i++) {
        cmap->subtable.startCodes[i] = atlib_bufread_read_u16(br);
    }
    for(u16 i = 0; i < segs; i++) {
        cmap->subtable.idDeltas[i] = atlib_bufread_read_i16(br);
    }
    for(u16 i = 0; i < segs; i++) {
        cmap->subtable.idRangeOffsets[i] = atlib_bufread_read_u16(br);
    }
    for(u16 i = 0; i < cmap->subtable.length - 16 - (8 * segs); i+=2) {
        cmap->subtable.glyfIdArrays[i] = atlib_bufread_read_u16(br);
    }

    if(!(atlib_bufread_eof(br) || atlib_bufread_err(br))) goto ret;
arrays:
    free(buf);
encrec:
    free(cmap->encodingRecords);
err:
    cmap = NULL;
ret:
    return cmap;
}

void atlib_font_free_cmap(Cmap * cmap) {
    atlib_compassert(cmap);
    atlib_compassert(cmap->encodingRecords);
    atlib_compassert(cmap->subtable.endCodes);

    free(cmap->encodingRecords);
    free(cmap->subtable.endCodes);
}

#include "Atlib/bufread.h"
#include "Atlib/error.h"

#define __ATLIB_FONT_H
#include "AtFont/font_internal.h"

#include <stdio.h>
#include <stdlib.h>

static TableRecord * read_records(TableRecord * restrict rec, usize n, bufread_t * restrict br) {
    for(usize i = 0; !(atlib_bufread_err(br) || atlib_bufread_eof(br)) && i < n; i++) {
        for(u16 k = 0; k < 4; k++) {
            rec[i].tableTag[k] = atlib_bufread_read_u8(br);
        }
        rec[i].checksum = atlib_bufread_read_u32(br);
        rec[i].offset = atlib_bufread_read_u32(br);
        rec[i].length = atlib_bufread_read_u32(br);
    }
    return (atlib_bufread_err(br) || atlib_bufread_eof(br)) ? NULL : rec;
}

TableDirectory * atlib_font_read_directory(TableDirectory * restrict dir, bufread_t * restrict br) {
    atlib_compassert(dir);

    atlib_compassert(br);
    atlib_compassert(br->fh);

    dir->sfntVersion = atlib_bufread_read_u32(br);
    if(dir->sfntVersion != 0x00010000) {
        return NULL;
    }
    dir->numTables = atlib_bufread_read_u16(br);
    dir->searchRange = atlib_bufread_read_u16(br);
    dir->entrySelector = atlib_bufread_read_u16(br);
    dir->rangeShift = atlib_bufread_read_u16(br);
    if((dir->recs = malloc(dir->numTables * sizeof(TableRecord))) == NULL) return NULL;
    if(read_records(dir->recs, dir->numTables, br) == NULL) {
        free(dir->recs);
        return NULL;
    }
    return (atlib_bufread_err(br) || atlib_bufread_eof(br)) ? NULL : dir;
}

void atlib_font_free_directory(TableDirectory * dir) {
    atlib_compassert(dir);
    atlib_compassert(dir->recs);

    free(dir->recs);
}

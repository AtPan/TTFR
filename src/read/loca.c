#include "Atlib/bufread.h"
#include "Atlib/error.h"

#define __ATLIB_FONT_H
#include "AtFont/font_internal.h"

#include <stdlib.h>

Loca * atlib_font_read_loca(
        const TableDirectory * restrict dir,
        const           Head * restrict head,
        const           Maxp * restrict maxp,
                        Loca * restrict loca,
                   bufread_t * restrict br
) {
    TableRecord * rec;
    i16 format = head->indexToLocFormat;
    u32 * offsets;

	atlib_compassert(dir);
	atlib_compassert(head);
	atlib_compassert(maxp);
	atlib_compassert(loca);
	atlib_compassert(br); 
    
    if((rec = __find_tag_record(dir, "loca")) == NULL ||
        (offsets = malloc(sizeof(u32) * (maxp->numGlyphs + 1))) == NULL) goto err;
    atlib_bufread_seek(br, rec->offset);

    if(!format) {
        for(u16 i = 0; i < maxp->numGlyphs + 1; i++) {
            offsets[i] = (u32)atlib_bufread_read_u16(br) * 2;
        }
    }
    else if(format == 1) {
        for(u16 i = 0; i < maxp->numGlyphs + 1; i++) {
            offsets[i] = atlib_bufread_read_u32(br);
        }
    }
    else goto offset;

    loca->offset = offsets;
    if(!(atlib_bufread_eof(br) || atlib_bufread_err(br))) goto ret;
offset:
    free(offsets);
err:
    loca = NULL;
ret:
    return loca;
}

void atlib_font_free_loca(Loca * loca) {
    atlib_compassert(loca);
    atlib_compassert(loca->offset);

    free(loca->offset);
}

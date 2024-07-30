#include "AtFont.h"
#include "AtFont/font_table.h"
#include "AtFont/font_internal.h"

void atlib_font_render(const font_t * restrict font, const char * restrict str, u32 pt, u32 xy_off, SDL_Renderer * restrict render) {
    atlib_compassert(font);
    atlib_compassert(str);
    atlib_compassert(render);

    char c;

    while((c = *str++)) {
        const Glyf_optional_t g = atlib_font_to_glyf(font, c);

        if(!g.is_some) return;

        const Glyf glyf = Glyf_optional_unwrap(g);

        if(glyf.header.numberOfContours > 0) {
            atlib_render_simple_glyf(render, font, &glyf, pt, xy_off);
        }
        else {
            // Temp until complex glyf support is implemented
            atlib_assert(glyf.header.numberOfContours > 0);
        }
    }
}

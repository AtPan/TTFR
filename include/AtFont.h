#ifndef __ATLIB_FONT_H
#define __ATLIB_FONT_H

#include <SDL2/SDL_render.h>

#include "Atlib/bufread.h"
#include "Atlib/bufwrite.h"
#include "Atlib/error.h"

#ifdef NULL
#undef NULL
#define NULL ((void *)0)
#endif

typedef struct __font font_t;

extern const usize atlib_font_size(void);
font_t * atlib_font_open(font_t *__restrict font, bufread_t *__restrict br);
void atlib_font_free(font_t * font);

void atlib_font_render(const font_t *__restrict font, const char *__restrict str, u32 pt, u32 xy, SDL_Renderer *__restrict renderer);

#endif

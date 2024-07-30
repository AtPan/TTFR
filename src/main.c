#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "AtFont.h"
#include "AtFont/font_table.h"

int main(int, char **);
void display_font(font_t *, char, u16);

int main(int argc, char * argv[]) {
    const char * font_path = "./DejaVuSans.ttf";

    int code = 0;
    bufread_t br;
    font_t * font;

    atlib_error_open("out.txt", NULL);

    if(argc != 3) {
        atlib_log_fatal(aterr, "USAGE: %s <Point Size> <String>\n", argv[0]);
        code = 10;
        goto bad_args;
    }

    if(!(font = malloc(atlib_font_size()))) {
        atlib_log_fatal(aterr, "%s", "Failed to allocate memory for font buffer\n");
        code = 1;
        goto bad_args;
    }

    if(atlib_bufread_open(&br, font_path) == NULL) {
        atlib_log_fatal(aterr, "Failed to open buffered reader \"%s\"\n", font_path);
        code = 2;
        goto bad_bufread;
    }

    if(atlib_font_open(font, &br) == NULL) {
        atlib_log_fatal(aterr, "Failed to parse font \"%s\"\n", font_path);
        code = 3;
        goto bad_font;
    }

    display_font(font, argv[2][0], atoi(argv[1]));

    atlib_font_free(font);
    
bad_font:
    atlib_bufread_close(&br);

bad_bufread:
    free(font);

bad_args:
    atlib_error_close();
    return code;
}

void display_font(font_t * restrict font, char def, u16 pt) {
    const u32 SCREEN_WIDTH = 415;
    const u32 SCREEN_HEIGHT = 415;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * w = SDL_CreateWindow("Font", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer * render = SDL_CreateRenderer(w, -1, 0);
    SDL_Event e;

    char test[2] = {def, 0};

    while(1) {
        while(SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    goto quit;
                    break;
                case SDL_KEYDOWN:
                    if(e.key.keysym.sym == SDLK_ESCAPE) {
                        goto quit;
                    }
                    else {
                        test[0] = e.key.keysym.sym;
                    }
                    break;
            }
        }

        /* Clear Screen */
        SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
        SDL_RenderClear(render);
        SDL_SetRenderDrawColor(render, 255, 255, 255, 0);

        atlib_font_render(font, test, pt, 0, render);

        /* Present Glyfs */
        SDL_RenderPresent(render);
    }

quit:
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(w);
    SDL_Quit();
}

#include "window.h"
#include "settings.h"

SDL_Window* win;
SDL_Renderer* renderer;

WindowContext initWindow()
{
    int flags = 0;
    float font_scale = 1.0f;

    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
    SDL_Init(SDL_INIT_VIDEO);

    win = SDL_CreateWindow("Nuklear: Project Creator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    if (win == NULL) 
    {
        SDL_Log("Error SDL_CreateWindow %s", SDL_GetError());
        exit(-1);
    }

    flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

    renderer = SDL_CreateRenderer(win, -1, flags);
    if (renderer == NULL)
    {
        SDL_Log("Error SDL_CreateRenderer %s", SDL_GetError());
        exit(-1);
    }

    {
        int render_w, render_h;
        int window_w, window_h;
        float scale_x, scale_y;
        SDL_GetRendererOutputSize(renderer, &render_w, &render_h);
        SDL_GetWindowSize(win, &window_w, &window_h);
        scale_x = (float)(render_w) / (float)(window_w);
        scale_y = (float)(render_h) / (float)(window_h);
        SDL_RenderSetScale(renderer, scale_x, scale_y);
        font_scale = scale_y;
    }

    return (WindowContext){win, renderer, font_scale};
}

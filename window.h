#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;

    float font_scale;
} WindowContext;

WindowContext initWindow();

#endif // WINDOW_H

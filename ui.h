#ifndef UI_H 
#define UI_H

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "submodules/nuklear/nuklear.h"
#include "submodules/nuklear/demo/sdl_renderer/nuklear_sdl_renderer.h"

#include "settings.h"

void init_ui(SDL_Window* win, SDL_Renderer* renderer, float font_scale);
void build_options_ui(SupportedBuildSystem* build_system_choice);
void drawContent();
void whileLoop(SDL_Window* win, SDL_Renderer* renderer);

#endif // UI_H

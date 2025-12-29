#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <SDL2/SDL.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_SDL_RENDERER_IMPLEMENTATION
#define NK_IMPLEMENTATION
#include "submodules/nuklear/nuklear.h"
#include "submodules/nuklear/demo/sdl_renderer/nuklear_sdl_renderer.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

const char* c_main_content = "#include <stdio.h>\n\nint main(void)\n{\n\tprintf(\"Hello, World!\");\n\treturn 0;\n}";
const char* cpp_main_content = "#include <iostream>\n\nint main()\n{\n\tstd::cout << \"Hello, World!\" << std::endl;\n\treturn 0;\n}";
const char* go_main_content = "package main\n\nimport\"fmt\"\n\nfunc main() {\n\tfmt.Printf(\"Hello, World!\")\n}";
const char* zig_main_content = "const std = @import(\"std\");\n\npub fn main() !void {\n\tstd.debug.print(\"Hello, World\");\n}";

typedef enum {
    ERR_OK = 0,
    ERR_FILE_CREATION_FAILED,
    ERR_UNSUPPORTED_EXTENSION,
    ERR_INVALID_ARGUMENTS
} Error;

typedef struct {
    char* str;
    Error err;
} StringResult;

const char* supported_extensions[] = { ".c", ".cpp", ".go", ".zig" };

typedef enum {
    EXT_C,
    EXT_CPP,
    EXT_GO,
    EXT_ZIG
} SupportedExtension;

StringResult strcat_const(const char* str1, const char* str2)
{
    const size_t str1_len = strlen(str1);
    const size_t str2_len = strlen(str2);
    const size_t concat_len = str1_len + str2_len;

    char* buffer = malloc(concat_len) + 1;
    if (!buffer)
    {
        return (StringResult){ .str = NULL, .err = ERR_FILE_CREATION_FAILED };
    }

    memcpy(buffer, str1, str1_len);
    memcpy(buffer + str1_len, str2, str2_len);
    buffer[concat_len] = '\0';

    return (StringResult){ .str = buffer, .err = ERR_OK };
}

Error create_project(const char* filename, const char* extension)
{
    if (!filename || !extension || filename[0] == '\0' || extension[0] == '\0')
        return ERR_INVALID_ARGUMENTS;

    StringResult full_filename_res = strcat_const(filename, extension);

    FILE* f;

    f = fopen(full_filename_res.str, "w");

    if (strcmp(extension, supported_extensions[EXT_C]) == 0)
    {
        fprintf(f, c_main_content);
        return ERR_OK;
    }
    else if (strcmp(extension, supported_extensions[EXT_CPP])== 0)
    {
        fprintf(f, cpp_main_content);
        return ERR_OK;
    }
    else if (strcmp(extension, supported_extensions[EXT_GO]) == 0)
    {
        fprintf(f, go_main_content);
        return ERR_OK;
    }
    else if (strcmp(extension, supported_extensions[EXT_ZIG]) == 0)
    {
        fprintf(f, zig_main_content);
        return ERR_OK;
    }
    else
    {
        perror("We do not support this file extension yet...\n");
        return ERR_UNSUPPORTED_EXTENSION;
    }
    
    fclose(f);
}

void initWindow()
{
    SDL_Window* win;
    SDL_Renderer* renderer;
    int running = 1;
    int flags = 0;
    float font_scale = 1;

    const char* error_msg = "";
    struct nk_color err_msg_color;

    struct nk_context* ctx;
    struct nk_colorf bg;

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

    ctx = nk_sdl_init(win, renderer);
    {
        struct nk_font_atlas *atlas;
        struct nk_font_config config = nk_font_config(0);
        struct nk_font *font;

        nk_sdl_font_stash_begin(&atlas);
        font = nk_font_atlas_add_default(atlas, 13 * font_scale, &config);
        nk_sdl_font_stash_end();

        font->handle.height /= font_scale;
        nk_style_set_font(ctx, &font->handle);
    }

    nk_flags window_flags = NK_WINDOW_BORDER;

    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
    while (running)
    {
        SDL_Event evt;
        nk_input_begin(ctx);
        while (SDL_PollEvent(&evt))
        {
            if (evt.type == SDL_QUIT) running = 0;
            nk_sdl_handle_event(&evt);
        }

        nk_sdl_handle_grab();
        nk_input_end(ctx);

        if (nk_begin(ctx, "Project Creator", nk_rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT), window_flags))
        {
            static char filename[64];
            static char extension[8];

            nk_layout_row_dynamic(ctx, 30, 1);
            nk_label(ctx, "Enter the project filename / path (without extension):", NK_TEXT_LEFT);
            nk_edit_string_zero_terminated(ctx, NK_EDIT_SIMPLE, filename, sizeof(filename), nk_filter_default);

            nk_layout_row_dynamic(ctx, 30, 1);
            nk_label(ctx, "Enter the file extension (.c, .cpp, .go, .zig):", NK_TEXT_LEFT);
            nk_edit_string_zero_terminated(ctx, NK_EDIT_SIMPLE, extension, sizeof(extension), nk_filter_default);

            nk_layout_row_dynamic(ctx, 30, 1);
            if (nk_button_label(ctx, "Create Project"))
            {
                Error err = create_project(filename, extension);
                if (err == ERR_OK)
                {
                    error_msg = "Project created successfully!";
                    err_msg_color = nk_rgb(0, 255, 0);
                }
                else if (err == ERR_INVALID_ARGUMENTS)
                {
                    error_msg = "Error: Invalid arguments provided.";
                    err_msg_color = nk_rgb(255, 0, 0);
                }
                else if (err == ERR_UNSUPPORTED_EXTENSION)
                {
                    error_msg = "Error: Unsupported file extension.";
                    err_msg_color = nk_rgb(255, 0, 0);
                }
                else
                {
                    error_msg = "Error: Could not create the file.";
                    err_msg_color = nk_rgb(255, 0, 0);
                }
            }

            nk_layout_row_dynamic(ctx, 30, 1);
            nk_label_colored(ctx, error_msg, NK_TEXT_CENTERED, err_msg_color);

        }
        nk_end(ctx);

        SDL_SetRenderDrawColor(renderer, (Uint8)(bg.r * 255), (Uint8)(bg.g * 255), (Uint8)(bg.b * 255), (Uint8)(bg.a * 255));
        SDL_RenderClear(renderer);

        nk_sdl_render(NK_ANTI_ALIASING_ON);

        SDL_RenderPresent(renderer);
    }

    nk_sdl_shutdown();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

int main() 
{
    initWindow();

    return 0;
}

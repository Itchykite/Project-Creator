#include "ui.h"
#include "create_project.h"

#define NK_SDL_RENDERER_IMPLEMENTATION
#define NK_IMPLEMENTATION

struct nk_context* ctx;

void init_ui(SDL_Window* win, SDL_Renderer* renderer, float font_scale)
{
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
}

void build_options_ui(SupportedBuildSystem* build_system_choice)
{
    nk_layout_row_dynamic(ctx, 30, 2);
    nk_label(ctx, "Build System:", NK_TEXT_LEFT);
    if (nk_combo_begin_label(ctx, supported_build_systems[*build_system_choice], nk_vec2(200, 200)))
    {
        nk_layout_row_dynamic(ctx, 25, 1);
        const size_t build_system_count = supported_build_systems_count;
        for (size_t i = 0; i < build_system_count; i++)
        {
            if (nk_combo_item_label(ctx, supported_build_systems[i], NK_TEXT_LEFT))
            {
                *build_system_choice = (SupportedBuildSystem)i;
            }
        }
        nk_combo_end(ctx);
    }
}

void drawContent()
{
    nk_flags window_flags = NK_WINDOW_BORDER;

    ProjectOptions project_options;
    static int build_system_index = 0;

    static const char* error_msg = "";
    static struct nk_color err_msg_color;

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

        build_options_ui((SupportedBuildSystem*)&build_system_index);

        nk_layout_row_dynamic(ctx, 30, 1);
        if (nk_button_label(ctx, "Create Project"))
        {
            project_options = create_project(filename, extension, (SupportedBuildSystem)build_system_index);

            if (project_options.err == ERR_OK)
            {
                error_msg = "Project created successfully!";
                err_msg_color = nk_rgb(0, 255, 0);
            }
            else if (project_options.err ==  ERR_FILE_CREATION_FAILED)
            {
                error_msg = "Error: Failed to create project files.";
                err_msg_color = nk_rgb(255, 0, 0);
            }
            else if (project_options.err == ERR_UNSUPPORTED_EXTENSION)
            {
                error_msg = "Error: Unsupported file extension.";
                err_msg_color = nk_rgb(255, 0, 0);
            }
            else if (project_options.err == ERR_INVALID_ARGUMENTS)
            {
                error_msg = "Error: Invalid arguments provided.";
                err_msg_color = nk_rgb(255, 0, 0);
            }
            else if (project_options.err == ERR_INVALID_ARGUMENTS_BUILD_SYSTEM)
            {
                error_msg = "Error: Invalid build system for selected language.";
                err_msg_color = nk_rgb(255, 0, 0);
            }
            else
            {
                error_msg = "Error: An unknown error occurred.";
                err_msg_color = nk_rgb(255, 0, 0);
            }
        }
        
        if (error_msg[0] != '\0')
        {
            nk_layout_row_dynamic(ctx, 30, 1);
            nk_label_colored(ctx, error_msg, NK_TEXT_CENTERED, err_msg_color);
        }
    }
    nk_end(ctx);
}

void whileLoop(SDL_Window* win, SDL_Renderer* renderer)
{
    int running = 1;

    struct nk_colorf bg;

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

        drawContent();

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

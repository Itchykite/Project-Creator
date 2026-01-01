#include "settings.h"
#include "ui.h"

#include <stdio.h>

const char c_main_content[] = "#include <stdio.h>\n"
                              "\n"
                              "int main(void)\n"
                              "{\n"
                              "    printf(\"Hello, World!\\n\");"
                              "\n\treturn 0;"
                              "\n}";

const char cpp_main_content[] = "#include <iostream>\n"
                                "\n"
                                "int main()\n"
                                "{\n"
                                "    std::cout << \"Hello, World!\" << std::endl;\n"
                                "\treturn 0;\n"
                                "}";

const char go_main_content[] = "package main\n"
                               "\n"
                               "import \"fmt\"\n"
                               "\n"
                               "func main() {\n"
                               "    fmt.Println(\"Hello, World!\\n\")\n"
                               "}";

const char zig_main_content[] = "const std = @import(\"std\");\n"
                                "\n"
                                "pub fn main() void {\n"
                                "    std.debug.print(\"Hello, World!\\n\", .{});\n"
                                "}";

const char sdl2_c_template[] = "#include <SDL2/SDL.h>\n"
                               "#include <stdio.h>\n"
                               "\n"
                               "int main(int argc, char* argv[])\n"
                               "{\n"
                               "    if (SDL_Init(SDL_INIT_VIDEO) != 0)\n"
                               "    {\n"
                               "        printf(\"SDL_Init Error: %s\\n\", SDL_GetError());\n"
                               "        return 1;\n"
                               "    }\n\n"
                               "    SDL_Window *win = SDL_CreateWindow(\"Hello World!\", 100, 100, 640, 480, SDL_WINDOW_SHOWN);\n"
                               "    if (win == NULL)\n"
                               "    {\n"
                               "        printf(\"SDL_CreateWindow Error: %s\\n\", SDL_GetError());\n"
                               "        SDL_Quit();\n"
                               "        return 1;\n"
                               "    }\n\n"
                               "    bool running = true;\n\n"
                               "    while (running)\n"
                               "    {\n"
                               "        SDL_Event event;\n"
                               "        while(SDL_PollEvent(&event))\n"
                               "        {\n"
                               "            if (event.type == SDL_QUIT)\n"
                               "            {\n"
                               "                running = false;\n"
                               "            }\n"
                               "        }\n"
                               "    }\n\n"
                               "    SDL_DestroyWindow(win);\n"
                               "    SDL_Quit();\n"
                               "    return 0;\n"
                               "}";

const char sdl2_cpp_template[] = "#include <SDL2/SDL.h>\n"
                                 "#include <iostream>\n"
                                 "\n"
                                 "int main(int argc, char* argv[])\n"
                                 "{\n"
                                 "    if (SDL_Init(SDL_INIT_VIDEO) != 0)\n"
                                 "    {\n"
                                 "        std::cout << \"SDL_Init Error: \" << SDL_GetError() << std::endl;\n"
                                 "        return 1;\n"
                                 "    }\n\n"
                                 "    SDL_Window *win = SDL_CreateWindow(\"Hello World!\", 100, 100, 640, 480, SDL_WINDOW_SHOWN);\n"
                                 "    if (win == NULL)\n"
                                 "    {\n"
                                 "        std::cout << \"SDL_CreateWindow Error: \" << SDL_GetError() << std::endl;\n"
                                 "        SDL_Quit();\n"
                                 "        return 1;\n"
                                 "    }\n\n"
                                 "    bool running = true;\n\n"
                                 "    while (running)\n"
                                 "    {\n"
                                 "        SDL_Event event;\n"
                                 "        while(SDL_PollEvent(&event))\n"
                                 "        {\n"
                                 "            if (event.type == SDL_QUIT)\n"
                                 "            {\n"
                                 "                running = false;\n"
                                 "            }\n"
                                 "        }\n"
                                 "    }\n\n"
                                 "    SDL_DestroyWindow(win);\n"
                                 "    SDL_Quit();\n"
                                 "    return 0;\n"
                                 "}";

const char* const supported_extensions[] = {".c", ".cpp", ".go", ".zig"};
const size_t supported_extensions_count = sizeof(supported_extensions) / sizeof(supported_extensions[0]);
const char* const supported_build_systems[] = {"Makefile", "CMake"};
const size_t supported_build_systems_count = sizeof(supported_build_systems) / sizeof(supported_build_systems[0]);
const char* const supported_templates[] = {"main", "sdl2"};
const size_t supported_templates_count = sizeof(supported_templates) / sizeof(supported_templates[0]);

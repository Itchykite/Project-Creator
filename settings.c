#include "settings.h"
#include "ui.h"

#include <stdio.h>

char* c_main_content = NULL;
char* cpp_main_content = NULL;
char* go_main_content = NULL;
char* zig_main_content = NULL;

char* sdl2_c_template = NULL;
char* sdl2_cpp_template = NULL;

FileResult read_file(const char* filename)
{
    FileResult result = {NULL, ERR_OK};

    FILE* file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Error: Could not open file '%s'\n", filename);
        result.err = ERR_FILE_CREATION_FAILED;
        return result;
    }

    // Znajdź rozmiar pliku
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size < 0)
    {
        fprintf(stderr, "Error: Could not determine file size\n");
        fclose(file);
        result.err = ERR_FILE_CREATION_FAILED;
        return result;
    }

    // Alokuj pamięć
    result.content = malloc(file_size + 1);
    if (!result.content)
    {
        fprintf(stderr, "Error:  Memory allocation failed\n");
        fclose(file);
        result.err = ERR_FILE_CREATION_FAILED;
        return result;
    }

    // Wczytaj plik
    size_t bytes_read = fread(result.content, 1, file_size, file);
    result.content[bytes_read] = '\0';

    fclose(file);
    return result;
}

void init_templates()
{
    if (c_main_content == NULL)
    {
        FileResult res = read_file("templates/c_main_content.c");
        if (res.err == ERR_OK)
        {
            c_main_content = res.content;
        }
    }

    if (cpp_main_content == NULL)
    {
        FileResult res = read_file("templates/cpp_main_content.cpp");
        if (res.err == ERR_OK)
        {
            cpp_main_content = res.content;
        }
    }

    if (go_main_content == NULL)
    {
        FileResult res = read_file("templates/go_main_content.go");
        if (res.err == ERR_OK)
        {
            go_main_content = res.content;
        }
    }

    if (zig_main_content == NULL)
    {
        FileResult res = read_file("templates/zig_main_content.zig");
        if (res.err == ERR_OK)
        {
            zig_main_content = res.content;
        }
    }

    if (sdl2_c_template == NULL)
    {
        FileResult res = read_file("templates/c_sdl2_content.c");
        if (res.err == ERR_OK)
        {
            sdl2_c_template = res.content;
        }
    }

    if (sdl2_cpp_template == NULL)
    {
        FileResult res = read_file("templates/cpp_sdl2_content.cpp");
        if (res.err == ERR_OK)
        {
            sdl2_cpp_template = res.content;
        }
    }
}

void free_templates()
{
    free(c_main_content);
    free(cpp_main_content);
    free(go_main_content);
    free(zig_main_content);
    free(sdl2_c_template);
    free(sdl2_cpp_template);
}

const char* const supported_extensions[] = {".c", ".cpp", ".go", ".zig"};
const size_t supported_extensions_count = sizeof(supported_extensions) / sizeof(supported_extensions[0]);
const char* const supported_build_systems[] = {"Makefile", "CMake"};
const size_t supported_build_systems_count = sizeof(supported_build_systems) / sizeof(supported_build_systems[0]);
const char* const supported_templates[] = {"main", "sdl2"};
const size_t supported_templates_count = sizeof(supported_templates) / sizeof(supported_templates[0]);

#include "create_project.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

StringResult strcat_const(const char* str1, const char* str2)
{
    const size_t str1_len = strlen(str1);
    const size_t str2_len = strlen(str2);
    const size_t concat_len = str1_len + str2_len;

    char* buffer = malloc(concat_len + 1);
    if (!buffer)
    {
        return (StringResult){ .str = NULL, .err = ERR_FILE_CREATION_FAILED };
    }

    memcpy(buffer, str1, str1_len);
    memcpy(buffer + str1_len, str2, str2_len);
    buffer[concat_len] = '\0';

    return (StringResult){ .str = buffer, .err = ERR_OK };
}

ProjectOptions create_project(const char* filename, const char* extension, SupportedBuildSystem build_system)
{
    ProjectOptions options = {0};
    options.err = ERR_OK;
    options.filename = NULL;

    if (!filename || !extension || filename[0] == '\0' || extension[0] == '\0')
    {
        fprintf(stderr, "Error: Invalid arguments provided for project creation.\n");
        options.err = ERR_INVALID_ARGUMENTS;
        return options;
    }

    SupportedExtension ext_type;
    if (strcmp(extension, supported_extensions[EXT_C]) == 0) ext_type = EXT_C;
    else if (strcmp(extension, supported_extensions[EXT_CPP]) == 0) ext_type = EXT_CPP;
    else if (strcmp(extension, supported_extensions[EXT_GO]) == 0) ext_type = EXT_GO;
    else if (strcmp(extension, supported_extensions[EXT_ZIG]) == 0) ext_type = EXT_ZIG;
    else
    {
        fprintf(stderr, "Error: Unsupported file extension '%s'.\n", extension);
        options.err = ERR_UNSUPPORTED_EXTENSION;
        return options;
    }

    if (build_system == BUILD_CMAKE && ext_type != EXT_CPP)
    {
        fprintf(stderr, "Error: CMake is only supported for C++ projects.\n");
        options.err = ERR_INVALID_ARGUMENTS_BUILD_SYSTEM;
        return options;
    }

    StringResult full_filename_res = strcat_const(filename, extension);
    if (full_filename_res.err != ERR_OK)
    {
        fprintf(stderr, "Error: Failed to allocate memory for filename.\n");
        options.err = ERR_FILE_CREATION_FAILED;
        return options;
    }

    FILE* f_main = fopen(full_filename_res.str, "w");
    if (!f_main)
    {
        fprintf(stderr, "Error: Failed to create main source file '%s'.\n", full_filename_res.str);
        free(full_filename_res.str);
        options.err = ERR_FILE_CREATION_FAILED;
        return options;
    }

    switch (ext_type)
    {
        case EXT_C:    fprintf(f_main, "%s", c_main_content); break;
        case EXT_CPP:  fprintf(f_main, "%s", cpp_main_content); break;
        case EXT_GO:   fprintf(f_main, "%s", go_main_content); break;
        case EXT_ZIG:  fprintf(f_main, "%s", zig_main_content); break;
    }

    fclose(f_main);
    free(full_filename_res.str);
    options.extension = ext_type;

    const char* build_filename = NULL;
    const char* build_content = NULL;

    if (build_system == BUILD_MAKEFILE)
    {
        build_filename = "Makefile";
        if (ext_type == EXT_C) build_content = c_main_makefile;
        else if (ext_type == EXT_CPP) build_content = cpp_main_makefile;
    }
    else if (build_system == BUILD_CMAKE)
    {
        build_filename = "CMakeLists.txt";
        if (ext_type == EXT_CPP) build_content = cpp_main_cmake;
    }

    if (build_filename && build_content)
    {
        FILE* f_build = fopen(build_filename, "w");
        if (!f_build)
        {
            fprintf(stderr, "Error: Failed to create build file '%s'.\n", build_filename);
            options.err = ERR_FILE_CREATION_FAILED;
            return options;
        }
        fprintf(f_build, "%s", build_content);
        fclose(f_build);
    }

    options.build_system = build_system;
    options.err = ERR_OK;
    return options;
}

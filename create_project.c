#include "create_project.h"
#include "settings.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

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

ProjectOptions create_project(const char* project_name, SupportedExtension extension, SupportedBuildSystem build_system)
{
    ProjectOptions options = {0};
    options.extension = extension;
    options.build_system = build_system;

    if (!project_name || project_name[0] == '\0' || extension >= supported_extensions_count)
    {
        fprintf(stderr, "Error: Invalid arguments provided.\n");
        options.err = ERR_INVALID_ARGUMENTS;
        return options;
    }

    struct stat st = {0};
    if (stat(project_name, &st) != -1)
    {
        fprintf(stderr, "Error: Directory '%s' already exists.\n", project_name);
        options.err = ERR_DIRECTORY_ALREADY_EXISTS;
        return options;
    }

    if (mkdir(project_name, 0755) != 0)
    {
        fprintf(stderr, "Error: Failed to create directory '%s'.\n", project_name);
        options.err = ERR_DIRECTORY_CREATION_FAILED;
        return options;
    }

    char main_filepath[256];
    snprintf(main_filepath, sizeof(main_filepath), "%s/main%s", project_name, supported_extensions[extension]);

    FILE* f_main = fopen(main_filepath, "w");
    if (!f_main)
    {
        fprintf(stderr, "Error: Failed to create main source file '%s'.\n", main_filepath);
        options.err = ERR_FILE_CREATION_FAILED;
        return options;
    }

    const char* main_content = NULL;
    switch (extension)
    {
        case EXT_C:    main_content = c_main_content; break;
        case EXT_CPP:  main_content = cpp_main_content; break;
        case EXT_GO:   main_content = go_main_content; break;
        case EXT_ZIG:  main_content = zig_main_content; break;
        default: break; 
    }
    fprintf(f_main, "%s", main_content);
    fclose(f_main);

    if (extension == EXT_GO || extension == EXT_ZIG)
    {
        char command[512];
        if (extension == EXT_GO) 
        {
            snprintf(command, sizeof(command), "cd %s && go mod init %s", project_name, project_name);
        } 
        else if (extension == EXT_ZIG) 
        { 
            snprintf(command, sizeof(command), "rm %s && cd %s && zig init-exe", main_filepath, project_name);
        }
        
        if (system(command) != 0) {
            fprintf(stderr, "Warning: Failed to run init command for the language.\n");
        }
        options.err = ERR_OK;
        return options;
    }
    
    const char* build_filename_part = NULL;
    const char* build_content = NULL;

    if (build_system == BUILD_MAKEFILE) 
    {
        build_filename_part = "Makefile";
        if (extension == EXT_C) build_content = c_main_makefile;
        else if (extension == EXT_CPP) build_content = cpp_main_makefile;
    } 
    else if (build_system == BUILD_CMAKE) 
    {
        build_filename_part = "CMakeLists.txt";
        if (extension == EXT_CPP) build_content = cpp_main_cmake;
        else if (extension == EXT_C) build_content = c_main_cmake;
    }

    if (build_filename_part && build_content) 
    {
        char build_filepath[256];
        snprintf(build_filepath, sizeof(build_filepath), "%s/%s", project_name, build_filename_part);

        FILE* f_build = fopen(build_filepath, "w");
        if (!f_build) 
        {
            fprintf(stderr, "Error: Failed to create build file '%s'.\n", build_filepath);
            options.err = ERR_FILE_CREATION_FAILED;
            return options;
        }

        fprintf(f_build, "%s", build_content);
        fclose(f_build);
    }
    
    options.err = ERR_OK;
    return options;
}

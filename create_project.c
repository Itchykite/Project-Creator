#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "create_project.h"
#include "settings.h"
#include "utils.h"

ProjectOptions create_project(const char* project_name, const char* filename, SupportedExtension extension,
                              SupportedBuildSystem build_system, SupportedTemplates template_type,
                              const char* extra_flags)
{
    ProjectOptions options = {0};
    options.extension = extension;
    options.build_system = build_system;
    options.template_type = template_type;

    if (!filename || filename[0] == '\0' || !project_name || project_name[0] == '\0' ||
        extension >= supported_extensions_count)
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

    ProjectTemplateProperties template_props =
        create_template(project_name, filename, extension, build_system, template_type, extra_flags);
    options.err = template_props.err;

    return options;
}

ProjectTemplateProperties create_template(const char* project_name, const char* filename, SupportedExtension extension,
                                          SupportedBuildSystem build_system, SupportedTemplates template_type,
                                          const char* extra_flags)
{
    ProjectTemplateProperties properties = {0};
    properties.flags = "";

    switch (template_type)
    {
        case MAIN_TEMPLATE:
            switch (extension)
            {
                case EXT_C:
                    properties.code = c_main_content;
                    break;
                case EXT_CPP:
                    properties.code = cpp_main_content;
                    break;
                default:
                    properties.code = "";
                    break;
            }
            break;
        case SDL2_TEMPLATE:
            switch (extension)
            {
                case EXT_C:
                    properties.code = sdl2_c_template;
                    properties.flags = "-lSDL2";
                    break;
                case EXT_CPP:
                    properties.code = sdl2_cpp_template;
                    properties.flags = "-lSDL2";
                    break;
                default:
                    properties.code = "";
                    break;
            }
            break;
        case SDL3_TEMPLATE:
            switch (extension)
            {
                case EXT_C:
                    properties.code = sdl3_c_template;
                    properties.flags = "-lSDL3";
                    break;
                case EXT_CPP:
                    properties.code = sdl3_cpp_template;
                    properties.flags = "-lSDL3";
                    break;
                default:
                    properties.code = "";
                    break;
            }
            break;
        default:
            properties.code = "";
            break;
    }

    char main_filepath[256];
    snprintf(main_filepath, sizeof(main_filepath), "%s/%s%s", project_name, filename, supported_extensions[extension]);

    FILE* f_main = fopen(main_filepath, "w");
    if (!f_main)
    {
        fprintf(stderr, "Error: Failed to create main source file '%s'.\n", main_filepath);
        properties.err = ERR_FILE_CREATION_FAILED;
        return properties;
    }

    fprintf(f_main, "%s", properties.code);
    fclose(f_main);

    ConstStringResult build_result =
        build_system_filename(project_name, filename, build_system, extension, extra_flags ? extra_flags : "");
    if (build_result.err != ERR_OK)
    {
        properties.err = build_result.err;
        return properties;
    }

    const char* build_filename_part = NULL;
    const char* build_content = build_result.str;

    if (build_system == BUILD_MAKEFILE)
    {
        build_filename_part = "Makefile";
    }
    else if (build_system == BUILD_CMAKE)
    {
        build_filename_part = "CMakeLists.txt";
    }
    else
    {
        build_filename_part = NULL;
        build_content = NULL;
    }

    if (build_filename_part && build_content)
    {
        char build_filepath[256];
        snprintf(build_filepath, sizeof(build_filepath), "%s/%s", project_name, build_filename_part);

        FILE* f_build = fopen(build_filepath, "w");
        if (!f_build)
        {
            fprintf(stderr, "Error: Failed to create build file '%s'.\n", build_filepath);
            properties.err = ERR_FILE_CREATION_FAILED;
            return properties;
        }

        fprintf(f_build, "%s", build_content);
        fclose(f_build);
    }

    properties.err = ERR_OK;
    return properties;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "create_project.h"
#include "settings.h"
#include "utils.h"

ProjectOptions create_project(const char* project_name, const char* filename, SupportedExtension extension, SupportedBuildSystem build_system, SupportedTemplates template_type,
                              const char* extra_flags)
{
    ProjectOptions options = {0};
    options.extension = extension;
    options.build_system = build_system;
    options.template_type = template_type;

    if (!filename || filename[0] == '\0' || !project_name || project_name[0] == '\0' || extension >= supported_extensions_count)
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

    ProjectTemplateProperties template_props = create_project_template(project_name, filename, extension, build_system, template_type, extra_flags);
    options.err = template_props.err;

    return options;
}

ProjectTemplateProperties create_project_template(const char* project_name, const char* filename, SupportedExtension extension, SupportedBuildSystem build_system, SupportedTemplates template_type,
                                                  const char* extra_flags)
{
    ProjectTemplateProperties properties = {0};

    switch (template_type)
    {
        case MAIN_TEMPLATE:
            switch (extension)
            {
                case EXT_C:
                    properties = create_main_c_template(project_name, filename, extension, build_system, extra_flags);
                    break;
                case EXT_CPP:
                    properties = create_main_cpp_template(project_name, filename, extension, build_system, extra_flags);
                    break;
                case EXT_GO:
                    properties = create_main_go_template(project_name, filename);
                    break;
                case EXT_ZIG:
                    properties = create_main_zig_template(project_name, filename);
                    break;
                default:
                    fprintf(stderr, "Error:  Unsupported file extension for main template.\n");
                    properties.err = ERR_UNSUPPORTED_EXTENSION;
                    break;
            }
            break;

        case SDL2_TEMPLATE:
            switch (extension)
            {
                case EXT_C:
                    properties = create_sdl2_c_template(project_name, filename, extension, build_system, extra_flags);
                    break;
                case EXT_CPP:
                    properties = create_sdl2_cpp_template(project_name, filename, extension, build_system, extra_flags);
                    break;
                default:
                    fprintf(stderr, "Error:  Unsupported file extension for SDL2 template.\n");
                    properties.err = ERR_UNSUPPORTED_EXTENSION;
                    break;
            }
            break;

        case SDL3_TEMPLATE:
            switch (extension)
            {
                case EXT_C:
                    properties = create_sdl3_c_template(project_name, filename, extension, build_system, extra_flags);
                    break;
                case EXT_CPP:
                    properties = create_sdl3_cpp_template(project_name, filename, extension, build_system, extra_flags);
                    break;
                default:
                    fprintf(stderr, "Error:  Unsupported file extension for SDL3 template.\n");
                    properties.err = ERR_UNSUPPORTED_EXTENSION;
                    break;
            }
            break;

        default:
            fprintf(stderr, "Error:  Unsupported template type.\n");
            properties.err = ERR_INVALID_ARGUMENTS;
            break;
    }

    return properties;
}

ProjectTemplateProperties create_main_c_template(const char* project_name, const char* filename, SupportedExtension extension, SupportedBuildSystem build_system, const char* extra_flags)
{
    ProjectTemplateProperties properties = {0};
    properties.flags = "";
    properties.code = c_main_content;

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

    ConstStringResult build_result = build_system_filename(project_name, filename, build_system, extension, extra_flags ? extra_flags : "");
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

ProjectTemplateProperties create_main_cpp_template(const char* project_name, const char* filename, SupportedExtension extension, SupportedBuildSystem build_system, const char* extra_flags)
{
    ProjectTemplateProperties properties = {0};
    properties.flags = "";
    properties.code = cpp_main_content;

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

    ConstStringResult build_result = build_system_filename(project_name, filename, build_system, extension, extra_flags ? extra_flags : "");
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

ProjectTemplateProperties create_main_go_template(const char* project_name, const char* filename)
{
    ProjectTemplateProperties properties = {0};
    properties.flags = "";
    properties.code = go_main_content;

    char main_filepath[256];
    snprintf(main_filepath, sizeof(main_filepath), "%s/%s%s", project_name, filename, supported_extensions[EXT_GO]);

    FILE* f_main = fopen(main_filepath, "w");
    if (!f_main)
    {
        fprintf(stderr, "Error: Failed to create main source file '%s'.\n", main_filepath);
        properties.err = ERR_FILE_CREATION_FAILED;
        return properties;
    }

    fprintf(f_main, "%s", properties.code);
    fclose(f_main);

    char command[512];
    snprintf(command, sizeof(command), "cd %s && go mod init %s", project_name, filename);

    if (system(command) != 0)
    {
        fprintf(stderr, "Warning: Failed to run 'go mod init'.\n");
    }

    properties.err = ERR_OK;
    return properties;
}

ProjectTemplateProperties create_main_zig_template(const char* project_name, const char* filename)
{
    ProjectTemplateProperties properties = {0};
    properties.flags = "";
    properties.code = zig_main_content;

    char main_filepath[256];
    snprintf(main_filepath, sizeof(main_filepath), "%s/%s%s", project_name, filename, supported_extensions[EXT_ZIG]);

    char command[512];
    snprintf(command, sizeof(command), "cd %s && zig init-exe", project_name);

    if (system(command) != 0)
    {
        fprintf(stderr, "Warning: Failed to run 'zig init-exe'.\n");
    }

    properties.err = ERR_OK;
    return properties;
}

ProjectTemplateProperties create_sdl2_c_template(const char* project_name, const char* filename, SupportedExtension extension, SupportedBuildSystem build_system, const char* extra_flags)
{
    ProjectTemplateProperties properties = {0};
    properties.flags = "-lSDL2";
    properties.code = sdl2_c_template;

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

    char combined_flags[512];
    snprintf(combined_flags, sizeof(combined_flags), "%s %s", properties.flags, extra_flags ? extra_flags : "");
    printf("Combined Flags: %s\n", combined_flags);

    ConstStringResult build_result = build_system_filename(project_name, filename, build_system, extension, combined_flags);
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

ProjectTemplateProperties create_sdl2_cpp_template(const char* project_name, const char* filename, SupportedExtension extension, SupportedBuildSystem build_system, const char* extra_flags)
{
    ProjectTemplateProperties properties = {0};
    properties.flags = "-lSDL2";
    properties.code = sdl2_cpp_template;

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

    char combined_flags[512];
    snprintf(combined_flags, sizeof(combined_flags), "%s %s", properties.flags, extra_flags ? extra_flags : "");

    ConstStringResult build_result = build_system_filename(project_name, filename, build_system, extension, combined_flags);
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

ProjectTemplateProperties create_sdl3_c_template(const char* project_name, const char* filename, SupportedExtension extension, SupportedBuildSystem build_system, const char* extra_flags)
{
    ProjectTemplateProperties properties = {0};
    properties.flags = "-lSDL3";
    properties.code = sdl3_c_template;

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

    char combined_flags[512];
    snprintf(combined_flags, sizeof(combined_flags), "%s %s", properties.flags, extra_flags ? extra_flags : "");

    ConstStringResult build_result = build_system_filename(project_name, filename, build_system, extension, combined_flags);
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

ProjectTemplateProperties create_sdl3_cpp_template(const char* project_name, const char* filename, SupportedExtension extension, SupportedBuildSystem build_system, const char* extra_flags)
{
    ProjectTemplateProperties properties = {0};
    properties.flags = "-lSDL3";
    properties.code = sdl3_cpp_template;

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

    char combined_flags[512];
    snprintf(combined_flags, sizeof(combined_flags), "%s %s", properties.flags, extra_flags ? extra_flags : "");

    ConstStringResult build_result = build_system_filename(project_name, filename, build_system, extension, combined_flags);
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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <stddef.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define BUILD_SYSTEM_COUNT 2

void init_templates();
void free_templates();

extern char* c_main_content;
extern char* sdl2_c_template;
extern char* sdl3_c_template;
extern char* sdl2_opengl_c_template;

extern char* cpp_main_content;
extern char* sdl2_cpp_template;
extern char* sdl3_cpp_template;

extern char* go_main_content;

extern char* zig_main_content;

extern const char* const supported_extensions[];
extern const size_t supported_extensions_count;
extern const char* const supported_build_systems[];
extern const size_t supported_build_systems_count;
extern const char* const supported_templates[];
extern const size_t supported_templates_count;

typedef enum {
    ERR_OK = 0,
    ERR_FILE_CREATION_FAILED,
    ERR_UNSUPPORTED_EXTENSION,
    ERR_INVALID_ARGUMENTS,
    ERR_INVALID_ARGUMENTS_BUILD_SYSTEM,
    ERR_DIRECTORY_ALREADY_EXISTS,
    ERR_DIRECTORY_CREATION_FAILED
} Error;

typedef enum {
    EXT_C,
    EXT_CPP,
    EXT_GO,
    EXT_ZIG
} SupportedExtension;

typedef enum {
    BUILD_MAKEFILE,
    BUILD_CMAKE
} SupportedBuildSystem;

typedef enum {
    MAIN_TEMPLATE,
    SDL2_TEMPLATE,
    SDL3_TEMPLATE,
    SDL2_OPENGL_TEMPLATE
} SupportedTemplates;

typedef struct {
    char* str;
    Error err;
} StringResult;

typedef struct {
    const char* str;
    Error err;
} ConstStringResult;

typedef struct {
    char* content;
    Error err;
} FileResult;

typedef struct {
    const char* str1;
    const char* str2;
} PairConstString;

typedef struct {
    const char* flags;
    const char* code;
    Error err;
} ProjectTemplateProperties;

typedef struct {
    char* filename;
    SupportedExtension extension;
    SupportedBuildSystem build_system;
    SupportedTemplates template_type;
    Error err;
} ProjectOptions;

bool is_template_available(SupportedTemplates template_type, SupportedExtension extension);

#endif // SETTINGS_H

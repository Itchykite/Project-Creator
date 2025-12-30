#ifndef SETTINGS_H
#define SETTINGS_H

#include <stddef.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define BUILD_SYSTEM_COUNT 2

extern const char c_main_content[];
extern const char c_main_makefile[];

extern const char cpp_main_content[];
extern const char cpp_main_makefile[];
extern const char cpp_main_cmake[];

extern const char go_main_content[];

extern const char zig_main_content[];

extern const char* const supported_extensions[];
extern const size_t supported_extensions_count;
extern const char* const supported_build_systems[];
extern const size_t supported_build_systems_count;

typedef enum {
    ERR_OK = 0,
    ERR_FILE_CREATION_FAILED,
    ERR_UNSUPPORTED_EXTENSION,
    ERR_INVALID_ARGUMENTS,
    ERR_INVALID_ARGUMENTS_BUILD_SYSTEM
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

typedef struct {
    char* str;
    Error err;
} StringResult;

typedef struct {
    char* filename;
    SupportedExtension extension;
    SupportedBuildSystem build_system;
    Error err;
} ProjectOptions;

#endif // SETTINGS_H

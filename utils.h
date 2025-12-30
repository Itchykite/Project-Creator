#ifndef UTILS_H
#define UTILS_H

#include "settings.h"

ConstStringResult build_system_filename(const char* project_name, const char* filename, SupportedBuildSystem build_system, SupportedExtension extension, const char* extra_flags);
StringResult strcat_const(const char* str1, const char* str2);
StringResult strcat_const_all(const char** strs);

#endif // UTILS_H

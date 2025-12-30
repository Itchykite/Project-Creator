#ifndef CREATE_PROJECT_H
#define CREATE_PROJECT_H

#include "settings.h"

StringResult strcat_const(const char* str1, const char* str2);

ProjectOptions create_project(const char* filename, const char* extension, SupportedBuildSystem build_system);

#endif // CREATE_PROJECT_H

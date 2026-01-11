#ifndef CREATE_PROJECT_H
#define CREATE_PROJECT_H

#include "settings.h"

ProjectOptions create_project(const char* project_name, const char* filename, SupportedExtension extension, SupportedBuildSystem build_system, SupportedTemplates template_type,
                              const char* extra_flags);

#endif // CREATE_PROJECT_H

#ifndef CREATE_PROJECT_H
#define CREATE_PROJECT_H

#include "settings.h"

ProjectOptions create_project(const char* project_name, const char* filename, SupportedExtension extension, SupportedBuildSystem build_system, SupportedTemplates template_type,
                              const char* extra_flags);

ProjectTemplateProperties create_project_template(const char* project_name, const char* filename, SupportedExtension extension, SupportedBuildSystem build_system, SupportedTemplates template_type,
                                                  const char* extra_flags);

ProjectTemplateProperties create_main_c_template(const char* project_name, const char* filename, SupportedExtension extension, SupportedBuildSystem build_system, const char* extra_flags);
ProjectTemplateProperties create_main_cpp_template(const char* project_name, const char* filename, SupportedExtension extension, SupportedBuildSystem build_system, const char* extra_flags);
ProjectTemplateProperties create_main_go_template(const char* project_name, const char* filename);
ProjectTemplateProperties create_main_zig_template(const char* project_name, const char* filename);

ProjectTemplateProperties create_sdl2_c_template(const char* project_name, const char* filename, SupportedExtension extension, SupportedBuildSystem build_system, const char* extra_flags);
ProjectTemplateProperties create_sdl2_cpp_template(const char* project_name, const char* filename, SupportedExtension extension, SupportedBuildSystem build_system, const char* extra_flags);
ProjectTemplateProperties create_sdl3_c_template(const char* project_name, const char* filename, SupportedExtension extension, SupportedBuildSystem build_system, const char* extra_flags);
ProjectTemplateProperties create_sdl3_cpp_template(const char* project_name, const char* filename, SupportedExtension extension, SupportedBuildSystem build_system, const char* extra_flags);

#endif // CREATE_PROJECT_H

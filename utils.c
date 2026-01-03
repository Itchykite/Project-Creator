#include "utils.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void split_flags(const char* flags, char* c_flags, char* ld_flags)
{
    c_flags[0] = '\0';
    ld_flags[0] = '\0';

    if (flags == NULL || strlen(flags) == 0)
    {
        return;
    }

    char* flags_copy = strdup(flags);
    if (!flags_copy)
    {
        fprintf(stderr, "Error: Memory allocation failed in split_flags\n");
        return;
    }

    char* token = strtok(flags_copy, " ");
    while (token != NULL)
    {
        if (token[0] == '-')
        {
            if (token[1] == 'l' || token[1] == 'L' || strncmp(token, "-Wl,", 4) == 0 || strcmp(token, "-pthread") == 0 || strcmp(token, "-static") == 0 || strcmp(token, "-shared") == 0)
            {
                strcat(ld_flags, token);
                strcat(ld_flags, " ");
            }
            else
            {
                strcat(c_flags, token);
                strcat(c_flags, " ");
            }
        }
        else
        {
            strcat(c_flags, token);
            strcat(c_flags, " ");
        }

        token = strtok(NULL, " ");
    }

    free(flags_copy);
}

ConstStringResult build_system_filename(const char* project_name, const char* filename, SupportedBuildSystem build_system, SupportedExtension extension, const char* extra_flags)
{
    static char out_build[512];

    char extra_c_flags[256] = "";
    char extra_ld_flags[256] = "";

    split_flags(extra_flags, extra_c_flags, extra_ld_flags);

    if (build_system == BUILD_MAKEFILE)
    {
        if (extension == EXT_C)
        {
            snprintf(out_build, sizeof(out_build),
                     "CC=gcc\n"
                     "CFLAGS=-I. %s\n"
                     "LDFLAGS= %s\n"
                     "\n"
                     "DEPS = %s.h\n"
                     "\n"
                     "OBJ = %s.o\n"
                     "\n"
                     "%%.o: %%.c $(DEPS)\n"
                     "\t$(CC) -c -o $@ $< $(CFLAGS)\n"
                     "\n"
                     "%s: $(OBJ)\n"
                     "\t$(CC) -o $@ $^ $(LDFLAGS)",
                     extra_c_flags, extra_ld_flags, filename, filename, project_name);
            return (ConstStringResult){out_build, ERR_OK};
        }
        else if (extension == EXT_CPP)
        {
            snprintf(out_build, sizeof(out_build),
                     "CXX=g++\n"
                     "CXXFLAGS=-I. %s\n"
                     "LDFLAGS= %s\n"
                     "\n"
                     "DEPS = %s.h\n"
                     "\n"
                     "OBJ = %s.o\n"
                     "\n"
                     "%%.o: %%.cpp $(DEPS)\n"
                     "\t$(CXX) -c -o $@ $< $(CXXFLAGS)\n"
                     "\n"
                     "%s:  $(OBJ)\n"
                     "\t$(CXX) -o $@ $^ $(LDFLAGS)",
                     extra_c_flags, extra_ld_flags, filename, filename, project_name);
            return (ConstStringResult){out_build, ERR_OK};
        }
    }
    else if (build_system == BUILD_CMAKE)
    {
        if (extension == EXT_C)
        {
            snprintf(out_build, sizeof(out_build),
                     "cmake_minimum_required(VERSION 3.10)\n"
                     "project(%s C)\n"
                     "\n"
                     "set(CMAKE_C_STANDARD 11)\n"
                     "\n"
                     "add_executable(%s %s. c)\n"
                     "target_link_libraries(%s %s)\n",
                     project_name, project_name, filename, project_name, extra_ld_flags);
            return (ConstStringResult){out_build, ERR_OK};
        }
        else if (extension == EXT_CPP)
        {
            snprintf(out_build, sizeof(out_build),
                     "cmake_minimum_required(VERSION 3.10)\n"
                     "project(%s CXX)\n"
                     "\n"
                     "set(CMAKE_CXX_STANDARD 11)\n"
                     "\n"
                     "add_executable(%s %s.cpp)\n"
                     "target_link_libraries(%s %s)\n",
                     project_name, project_name, filename, project_name, extra_ld_flags);
            return (ConstStringResult){out_build, ERR_OK};
        }
    }

    return (ConstStringResult){NULL, ERR_INVALID_ARGUMENTS};
}

StringResult strcat_const(const char* str1, const char* str2)
{
    const size_t str1_len = strlen(str1);
    const size_t str2_len = strlen(str2);
    const size_t concat_len = str1_len + str2_len;

    char* buffer = malloc(concat_len + 1);
    if (!buffer)
    {
        return (StringResult){.str = NULL, .err = ERR_FILE_CREATION_FAILED};
    }

    memcpy(buffer, str1, str1_len);
    memcpy(buffer + str1_len, str2, str2_len);
    buffer[concat_len] = '\0';

    return (StringResult){.str = buffer, .err = ERR_OK};
}

StringResult strcat_const_all(const char** strs)
{
    size_t total_length = 0;
    size_t count = 0;

    while (strs[count] != NULL)
    {
        total_length += strlen(strs[count]);
        count++;
    }

    char* buffer = malloc(total_length + 1);
    if (!buffer)
    {
        return (StringResult){.str = NULL, .err = ERR_FILE_CREATION_FAILED};
    }

    size_t offset = 0;
    for (size_t i = 0; i < count; i++)
    {
        size_t len = strlen(strs[i]);
        memcpy(buffer + offset, strs[i], len);
        offset += len;
    }
    buffer[total_length] = '\0';

    return (StringResult){.str = buffer, .err = ERR_OK};
}

#include "settings.h"

const char c_main_content[] = 
    "#include <stdio.h>\n"
    "\n"
    "int main(void)\n"
    "{\n"
    "\tprintf(\"Hello, World!\");"
    "\n\treturn 0;"
    "\n}";

const char c_main_makefile[] = 
    "CC=gcc\n"
    "CFLAGS=-I.\n"
    "\n"
    "DEPS = hellomake.h\n"
    "\n"
    "OBJ = hellomake.o hellofunc.o \n"
    "\n"
    "%.o: %.c $(DEPS)\n"
    "\t$(CC) -c -o $@ $< $(CFLAGS)\n"
    "\n"
    "hellomake: $(OBJ)\n"
    "\t$(CC) -o $@ $^ $(CFLAGS)";

const char cpp_main_content[] = 
    "#include <iostream>\n"
    "\n"
    "int main()\n" 
    "{\n"
    "\tstd::cout << \"Hello, World!\" << std::endl;\n"
    "\treturn 0;\n"
    "}";

const char cpp_main_makefile[] = 
    "CXX=g++\n"
    "CXXFLAGS=-I.\n"
    "\n"
    "DEPS = hellomake.h\n"
    "\n"
    "OBJ = hellomake.o hellofunc.o \n"
    "\n"
    "%.o: %.cpp $(DEPS)\n"
    "\t$(CXX) -c -o $@ $< $(CXXFLAGS)\n"
    "\n"
    "hellomake: $(OBJ)\n"
    "\t$(CXX) -o $@ $^ $(CXXFLAGS)";

const char cpp_main_cmake[] = 
    "cmake_minimum_required(VERSION 3.10)\n"
    "project(HelloWorld)\n"
    "\n"
    "set(CMAKE_CXX_STANDARD 11)\n"
    "\n"
    "add_executable(helloworld main.cpp)";

const char go_main_content[] = 
    "package main\n"
    "\n"
    "import \"fmt\"\n"
    "\n"
    "func main() {\n"
    "\tfmt.Println(\"Hello, World!\")\n"
    "}";

const char zig_main_content[] = 
    "const std = @import(\"std\");\n"
    "\n"
    "pub fn main() void {\n"
    "\tstd.debug.print(\"Hello, World!\\n\", .{});\n"
    "}";

const char* const supported_extensions[] = { ".c", ".cpp", ".go", ".zig" }; 
const size_t supported_extensions_count = sizeof(supported_extensions) / sizeof(supported_extensions[0]);
const char* const supported_build_systems[] = { "Makefile", "CMake" };
const size_t supported_build_systems_count = sizeof(supported_build_systems) / sizeof(supported_build_systems[0]);

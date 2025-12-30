#include "settings.h"

#include <stdio.h>

const char c_main_content[] = "#include <stdio.h>\n"
                              "\n"
                              "int main(void)\n"
                              "{\n"
                              "    printf(\"Hello, World!\\n\");"
                              "\n\treturn 0;"
                              "\n}";

const char cpp_main_content[] = "#include <iostream>\n"
                                "\n"
                                "int main()\n"
                                "{\n"
                                "    std::cout << \"Hello, World!\" << std::endl;\n"
                                "\treturn 0;\n"
                                "}";

const char go_main_content[] = "package main\n"
                               "\n"
                               "import \"fmt\"\n"
                               "\n"
                               "func main() {\n"
                               "    fmt.Println(\"Hello, World!\\n\")\n"
                               "}";

const char zig_main_content[] = "const std = @import(\"std\");\n"
                                "\n"
                                "pub fn main() void {\n"
                                "    std.debug.print(\"Hello, World!\\n\", .{});\n"
                                "}";

const char* const supported_extensions[] = {".c", ".cpp", ".go", ".zig"};
const size_t supported_extensions_count = sizeof(supported_extensions) / sizeof(supported_extensions[0]);
const char* const supported_build_systems[] = {"Makefile", "CMake"};
const size_t supported_build_systems_count = sizeof(supported_build_systems) / sizeof(supported_build_systems[0]);
const char* const supported_templates[] = {"main", "sdl2"};
const size_t supported_templates_count = sizeof(supported_templates) / sizeof(supported_templates[0]);

# Project Creator

A simple GUI application for quickly generating project templates with different programming languages, build systems, and frameworks.  

## Overview

Project Creator is a lightweight tool built with SDL2 and Nuklear that helps developers quickly scaffold new projects.  It provides an intuitive interface for creating projects in multiple languages (C, C++, Go, Zig) with various templates including SDL2 and SDL3 support.

## Features

### Supported Languages
- **C** (. c)
- **C++** (.cpp)
- **Go** (.go)
- **Zig** (.zig)

### Supported Build Systems
- **Makefile** - Traditional make-based build system
- **CMake** - Cross-platform build system generator

### Available Templates
- **Main Template** - Basic "Hello World" starter (available for all languages)
- **SDL2 Template** - SDL2 boilerplate with window and event loop (C/C++ only)
- **SDL3 Template** - SDL3 boilerplate with window and event loop (C/C++ only)

### Key Functionalities
- **Project Name Configuration** - Set custom project directory name
- **Filename Configuration** - Specify main source file name
- **Language Selection** - Choose from C, C++, Go, or Zig
- **Build System Selection** - Pick between Makefile or CMake (for C/C++)
- **Template Selection** - Select appropriate template based on language
- **Extra Build Flags** - Add custom compiler and linker flags
- **Automatic Build File Generation** - Creates Makefile or CMakeLists. txt automatically
- **Built-in Toolchain Support** - Go and Zig use their native build systems

## Compilation

### Prerequisites
- GCC or compatible C compiler
- SDL2 development libraries
- Make

### Build Instructions

1. **Clone the repository:**
```bash
git clone https://github.com/Itchykite/Project-Creator.git
cd Project-Creator
```

2. **Initialize submodules (for Nuklear):**
```bash
git submodule update --init --recursive
```

3. **Compile the project:**
```bash
make
```

4. **Run the application:**
```bash
./build/Project_Creator
```

### Build Requirements
- SDL2 (`sdl2-config` must be available)
- C11 standard compiler
- Math library (`-lm`)

## Usage

1. Launch the application
2. Enter your **project name** (this will be the directory name)
3. Enter the **filename** for your main source file
4. Select your desired **file extension/language**
5. Choose a **build system** (if applicable)
6. Select a **template type**
7. Optionally add **extra build flags** (e.g., `-Wall -O2 -lm`)
8. Click **"Create Project"**

The application will create a new directory with: 
- Main source file with template code
- Build system file (Makefile/CMakeLists.txt for C/C++)
- go.mod file (for Go projects)
- Zig build configuration (for Zig projects)

## Error Handling

The application provides clear error messages for common issues:
- **Directory already exists** - Project directory name conflicts with existing folder
- **Failed to create directory** - Permission or filesystem issues
- **Failed to create project files** - File writing errors
- **Unsupported file extension** - Invalid language/template combination
- **Invalid arguments** - Missing or incorrect parameters
- **Invalid build system** - Build system not supported for selected language

## Extra Build Flags

The extra flags field automatically separates compiler and linker flags:
- **Compiler flags**:  `-Wall`, `-O2`, `-g`, `-I<path>`, etc.
- **Linker flags**: `-l<lib>`, `-L<path>`, `-Wl,<option>`, `-pthread`, `-static`, `-shared`

Example:  `-Wall -O2 -lm -lpthread` will be properly split into CFLAGS and LDFLAGS in the generated Makefile.

## Author

Itchykite

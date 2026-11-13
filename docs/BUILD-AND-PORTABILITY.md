# Build and Portability

## Compiler requirements

The code uses standard C library headers: `stdio.h`, `stdlib.h`, and `string.h`. GCC and Clang are suitable compilers. The examples target C11 and do not require external dependencies.

## Build from source

Run commands from the repository root because the source path contains a space.

### GCC on Linux

```sh
mkdir -p build
gcc -std=c11 -Wall -Wextra -pedantic "TEXT EDITOR/main.c" -o build/text-editor
./build/text-editor
```

### Clang on macOS

```sh
mkdir -p build
clang -std=c11 -Wall -Wextra -pedantic "TEXT EDITOR/main.c" -o build/text-editor
./build/text-editor
```

### MinGW-w64 on Windows

```powershell
mkdir build
gcc -std=c11 -Wall -Wextra -pedantic "TEXT EDITOR/main.c" -o build\text-editor.exe
.\build\text-editor.exe
```

If your shell does not recognize `gcc`, install a C toolchain and ensure its executable directory is on `PATH`.

## Make targets

The repository Makefile provides:
- `make`: compile the editor into `build/text-editor`.
- `make run`: compile if necessary and launch the editor.
- `make clean`: remove the generated build directory.

Make is optional. Direct compiler commands work without it.

## Compiler warnings

Warnings should be treated as useful feedback. Do not suppress warnings simply to make CI green. When a warning is introduced, understand its cause and correct the source or document a justified compiler-specific exception.

The project workflow currently compiles with GCC using `-std=c11 -Wall -Wextra`. Local builds additionally use `-pedantic`.

## Platform considerations

### Terminal clearing

The program calls `system("cls")` on Windows and `system("clear")` on other platforms. This relies on an available shell command and may behave differently in IDE terminals or restricted environments.

### Color output

Keyword highlighting uses ANSI escape sequences. Modern terminals generally support them, but some older Windows consoles or redirected output may display escape characters literally.

### Executable compatibility

A compiled executable is specific to its target operating system and architecture. The checked-in `main.exe`, if present, should be considered an old build artifact. Recompile from `main.c` for the local environment.

## Reproducible builds

For reproducibility:
1. Record the compiler name and version.
2. Use the same C standard and warning flags.
3. Build from the checked-in source.
4. Avoid relying on a previously committed binary.
5. Keep generated output out of commits.

## Sanitizer build

Where supported, use:

```sh
gcc -std=c11 -Wall -Wextra -pedantic -g -fsanitize=address,undefined "TEXT EDITOR/main.c" -o build/text-editor-sanitized
```

This can expose out-of-bounds access and undefined behavior during manual testing. It does not prove that all input paths are safe.

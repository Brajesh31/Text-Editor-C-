# Troubleshooting

## The compiler says the source file cannot be found

The source file is inside a directory whose name contains a space. Run the compiler from the repository root and quote the path:

```sh
gcc -std=c11 -Wall -Wextra -pedantic "TEXT EDITOR/main.c" -o build/text-editor
```

Do not omit the quotation marks around `TEXT EDITOR/main.c`.

## The shell says gcc or clang is not recognized

Install a C compiler:
- Linux: install GCC through the distribution package manager.
- macOS: install Xcode Command Line Tools.
- Windows: install a MinGW-w64 distribution.

Ensure the compiler's executable directory is included in the system `PATH`. Open a new terminal after changing environment variables.

## The executable does not launch

Confirm that compilation completed successfully and that you are running the output path used by the compiler. On Linux and macOS, use `./build/text-editor`. In PowerShell, use `.\build\text-editor.exe`.

If the file exists but cannot execute, check its platform and architecture. A Windows executable cannot be run natively as a Linux binary.

## The screen does not clear

The current source calls `system("clear")` on non-Windows systems and `system("cls")` on Windows. The relevant command may not be installed or available in an IDE terminal. This is a known portability limitation.

## Colors appear as escape characters

Keyword highlighting prints ANSI escape sequences. Use a terminal with ANSI color support or redirect output to a plain text destination. The current application does not provide a color-disable option.

## A command is treated as text

The parser recognizes only a few exact command forms. An unknown or malformed command falls through to the add-line behavior. Check spelling, spacing, and the examples in [Command reference](COMMAND-REFERENCE.md).

## Replace does not change every occurrence

The current replacement function searches each line once and replaces only the first match in that line. It does not repeatedly replace every occurrence within the same line.

## Replace cannot search for a phrase

The current parser uses two single-word fields. Search and replacement arguments containing spaces are not supported.

## Undo does not behave independently after switching buffers

Undo and redo stacks are global, not per buffer. This is a known design limitation. Do not rely on switching buffers to isolate history.

## Text disappears after quitting

This is expected. The application stores text only in process memory and has no save or load command. All content is lost when the process exits.

## The program behaves strangely after end-of-file

The current input loop does not check the result of `fgets`. EOF handling is a known defect. Until fixed, avoid relying on redirected or piped input that ends unexpectedly.

## Report a reproducible defect

If the problem persists, open a GitHub issue using the bug report template. Include:
- Operating system and version.
- Compiler name and version.
- Exact build command.
- Exact input sequence.
- Expected and actual results.
- Any compiler warning or runtime diagnostic.

Do not include secrets or unrelated personal information.

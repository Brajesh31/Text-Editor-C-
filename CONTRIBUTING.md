# Contributing

Contributions that improve correctness, safety, usability, or documentation are welcome.

## Build locally

```bash
gcc -std=c11 -Wall -Wextra "TEXT EDITOR/main.c" -o text-editor
./text-editor
```

## Priorities

- Fix unsafe buffer and string handling.
- Add persistent open/save support without losing existing in-memory behavior.
- Add a help command and clearer error messages.
- Add tests for editing, undo/redo, buffer switching, and replacement.

## Pull requests

Keep changes focused. Describe the behavior changed, provide build/test output, and call out any changes to commands or limits. Do not commit generated executables.

# Command Reference

This page documents the command parser in `TEXT EDITOR/main.c`. It describes current behavior only. The application does not yet have a help command, command aliases, or a general-purpose command grammar.

## Input model

The program reads one input line at a time. It checks for a small set of command patterns. If none match, the input is passed to the add-line operation. This means a misspelled command may become document text rather than producing an error.

The current parser uses `fgets`, `strcmp`, and `sscanf`. The newline read by `fgets` is retained unless explicitly handled by a command path. Exact command comparisons therefore matter.

## Commands

### Add a line

Syntax: enter ordinary text and press Enter.

Any input that does not match another command is treated as a new line. The active buffer can store at most 100 lines. The source currently uses a 256-byte input and line slot size. Input truncation and null termination need stronger handling before this can be considered robust.

Example:

```text
This is the first note
This is another line
```

### Edit a line

Syntax:

```text
edit <line-number> <replacement text>
```

The line number is one-based. The replacement text is captured after the line number and one separating whitespace sequence.

Example:

```text
edit 2 Replace the second line
```

A valid line number is from 1 through the current line count. The current implementation saves an undo snapshot before checking validity, so invalid attempts can affect history.

### Undo

Syntax: `undo`

Restores a snapshot from the global undo stack. If no snapshot is available, the program prints a message. History is not separated by buffer.

### Redo

Syntax: `redo`

Restores a state from the redo stack. Redo entries are cleared when a new snapshot is saved. The history arrays have a fixed maximum of 50 entries.

### Replace

Syntax:

```text
replace <search-word> <replacement-word>
```

The parser accepts two single-word arguments. For each line, the function replaces the first matching occurrence. It does not replace every occurrence within the same line and does not support phrases containing spaces.

Important current limitations:
- The operation does not save an undo snapshot.
- Empty search strings are not explicitly rejected.
- Output length is not checked before concatenation into a fixed-size local array.
- The function mutates the buffer directly.

Do not use this operation on important or untrusted text until the string-handling defects are fixed.

### Switch buffer

Syntax:

```text
switch <buffer-number>
```

The accepted buffer numbers are 1 through 5. The number is converted to a zero-based array index internally.

Example:

```text
switch 3
```

The buffers are memory slots. They are not files and are not persisted to disk. Undo/redo history and indentation are global, so switching buffers does not switch all editor state independently.

### Quit

Syntax: `quit`

Exits the main loop. The program does not prompt to save because no save operation exists. All in-memory text is discarded.

## Unsupported commands

The current source does not implement:
- `help`
- `open` or `load`
- `save` or `save-as`
- `search` as a standalone command
- Multi-word search and replacement arguments
- Delete line, insert line, or move line
- Multiple independent undo histories
- Persistent command history

## Command parser improvement requirements

A safer parser should:
1. Remove or account for the trailing newline.
2. Detect input truncation rather than silently accepting partial commands.
3. Tokenize commands without writing beyond fixed buffers.
4. Reject malformed commands with a useful message.
5. Distinguish ordinary text from unknown command names.
6. Preserve spaces in text arguments where the command supports them.
7. Return a structured command value rather than mutating editor state during parsing.

Any command change should update this reference and the README.

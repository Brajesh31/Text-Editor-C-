# Limitations and Known Issues

This document records limitations visible in the current source. It is intentionally direct so that users and contributors do not mistake a learning prototype for a finished editor.

## Product limitations

- No file open, save, save-as, import, or export.
- No persistent storage. All text is lost at exit.
- No mouse support, menus, cursor movement, or screen-region editing.
- No delete, insert-at-position, copy, paste, or line movement command.
- No command help or command history.
- No multi-word find-and-replace arguments.
- No search highlighting or replace-all behavior within a line.
- No independent history per buffer.
- No full syntax highlighting or syntax-aware indentation.
- No automated behavioral test suite.

## Fixed capacities

The source defines:
- Five in-memory buffers.
- 100 lines per buffer.
- 256 bytes per line slot.
- 50 snapshots in each history array.

These are compile-time constants. Exceeding them must be handled explicitly. The current code does not provide a complete user-facing capacity policy for every path.

## Correctness and safety issues

### Input EOF handling

The return value of `fgets` is not checked. End-of-file should be detected and handled rather than allowing the loop to reuse old input.

### String termination and bounds

The program uses fixed-size character arrays. Calls to `strncpy` with the full destination size do not guarantee a trailing null byte when the source is at least that long. Every stored line should have a clearly enforced maximum length and explicit termination.

### Replacement buffer size

The replacement function concatenates prefix, replacement text, and suffix into a fixed local buffer. The combined output length is not calculated before concatenation. A longer replacement can exceed the destination capacity.

### History overflow and consistency

Undo and redo arrays have fixed capacity. The code does not define a safe rolling policy, and history is global across buffers. Every successful mutation should participate in history consistently.

### Invalid edit behavior

The edit function records a snapshot before validating the requested line number. Failed operations should not change history.

### Replacement history

Find-and-replace changes text without saving an undo snapshot. This makes the undo command inconsistent with other mutations.

### Indentation state

Indentation is a global counter, not a per-buffer field and not part of history snapshots. It may become inconsistent after undo or buffer switching. It also reacts to braces without understanding whether they occur in comments or string literals.

### Highlighting fidelity

The display routine uses tokenization with a small delimiter list. It is not a lexical analyzer and can alter the apparent spacing or handling of punctuation. It does not distinguish code from comments or string literals.

### Terminal behavior

The application invokes a shell command to clear the screen. This can fail when the command is unavailable or the process is running in a restricted environment.

## Recommended order of work

1. Make input handling safe, including EOF and truncated input.
2. Centralize bounded string operations.
3. Validate every command before changing state.
4. Make history per buffer and define overflow behavior.
5. Include indentation in buffer state or replace the counter with derived rendering.
6. Add tests for state operations.
7. Improve rendering and terminal portability.
8. Consider file persistence only after the in-memory model is reliable.

See [Roadmap](ROADMAP.md) for staged implementation planning.

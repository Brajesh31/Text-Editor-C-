# Project Roadmap

This roadmap is a proposal, not a statement that the listed work is already implemented. Priorities are ordered around correctness first, then usability and additional capabilities.

## Phase 1: Safety and predictable behavior

**Goal:** prevent invalid input and unsafe memory operations.

- Check the return value of `fgets` and exit cleanly on EOF.
- Detect input truncation and drain the remainder of an overlong input line.
- Guarantee null termination for every stored line.
- Replace unchecked concatenation with explicit output-length calculations.
- Reject empty search terms.
- Validate line and buffer indexes before state mutation.
- Ensure failed commands do not consume undo history.
- Provide clear feedback when a buffer or history limit is reached.

**Acceptance criteria:** no known fixed-buffer write occurs without a verified capacity; malformed input does not crash the process; boundary cases have regression tests.

## Phase 2: State and history correctness

**Goal:** make editing behavior consistent across buffers.

- Move undo/redo stacks into per-buffer state.
- Define whether history uses a rolling capacity or reports when full.
- Record every successful text mutation, including replacement.
- Keep indentation state consistent with undo and buffer switching.
- Avoid history entries for operations that do not change text.
- Add deterministic tests for history transitions.

**Acceptance criteria:** editing one buffer cannot cause undo to restore content from another buffer; every successful mutation has documented undo behavior.

## Phase 3: Command interface

**Goal:** make the command loop easier to learn and harder to misuse.

- Add a `help` command.
- Normalize newline handling.
- Separate parsing from editing operations.
- Distinguish unknown commands from ordinary text through an explicit input mode or command prefix.
- Improve error messages.
- Support multi-word replacement arguments if the parser design permits it.
- Add delete and insert-line operations with tests.

**Acceptance criteria:** malformed commands are rejected clearly; command syntax is documented and tested.

## Phase 4: Rendering and portability

**Goal:** reduce platform-specific behavior and improve display fidelity.

- Remove shell-based screen clearing or isolate it behind a platform interface.
- Make color output optional and detect non-interactive output where practical.
- Preserve punctuation and spacing exactly.
- Replace approximate tokenization with a deliberately scoped lexer if highlighting remains a project goal.
- Add Windows and Unix build checks.

**Acceptance criteria:** rendering does not mutate or lose document content; basic build checks run on supported platforms.

## Phase 5: File persistence

**Goal:** allow users to keep their work.

- Add explicit open and save commands.
- Define overwrite behavior and error handling.
- Handle file paths with spaces.
- Detect failed reads and writes.
- Avoid silently truncating large files.
- Decide whether the fixed line and character limits remain or are replaced by dynamic storage.
- Add tests for empty files, large files, missing paths, permission errors, and partial writes.

**Acceptance criteria:** saved content can be reopened and compared byte-for-byte within the supported text format; failures do not silently discard user content.

## Phase 6: Maintainability

**Goal:** keep the project understandable as it grows.

- Split input, editor state, history, rendering, and storage into modules.
- Add a small test harness.
- Document public function contracts and invariants.
- Add static analysis and sanitizer checks to CI.
- Establish release and versioning conventions only when the project is ready for releases.

## Scope discipline

A feature should not be added solely because it appears in a typical text editor. Each feature should have a clear user need, a bounded implementation, tests, and documentation. Memory safety and data integrity take priority over adding more commands.

# Contributing to the C Text Editor

Thank you for considering a contribution. The most useful contributions make the program safer, more predictable, easier to understand, or easier to test. A small, correct change is more valuable than a large feature that introduces memory errors or claims behavior the program does not actually support.

This guide explains the development environment, repository conventions, validation expectations, and pull request process.

## 1. Before starting

1. Read the [README](README.md) to understand the current feature set and limitations.
2. Read [Architecture](docs/ARCHITECTURE.md) before changing the main loop or editor state.
3. Check [Limitations](docs/LIMITATIONS.md) so existing defects and constraints are understood.
4. Search existing issues and pull requests before starting duplicate work.
5. For a significant behavior change, open an issue first and describe the intended behavior.

Do not assume the application supports file persistence, multiple independent histories, or a full syntax parser. Those features are not present in the current implementation.

## 2. Development environment

The project uses standard C and does not require third-party libraries.

Recommended tools:
- GCC or Clang with C11 support.
- Git.
- A terminal that can run an interactive command-line program.
- A debugger such as GDB or LLDB for investigating crashes.
- AddressSanitizer and UndefinedBehaviorSanitizer where supported.

Compile from the repository root:

```sh
mkdir -p build
gcc -std=c11 -Wall -Wextra -pedantic "TEXT EDITOR/main.c" -o build/text-editor
```

A sanitizer build can help identify memory defects:

```sh
gcc -std=c11 -Wall -Wextra -pedantic -g -fsanitize=address,undefined "TEXT EDITOR/main.c" -o build/text-editor-sanitized
```

Sanitizers are diagnostic tools, not a replacement for correct bounds checks and input validation.

## 3. Branch and commit conventions

Create a focused branch from the current default branch:

```sh
git checkout main
git pull --ff-only
git checkout -b fix/describe-the-change
```

Suggested branch prefixes:
- `fix/` for a defect correction.
- `feat/` for a user-visible capability.
- `docs/` for documentation-only changes.
- `test/` for tests and test infrastructure.
- `refactor/` for structural changes without intended behavior changes.

Use clear commit messages, for example:
- `fix: validate line input before copying`
- `docs: explain buffer history behavior`
- `test: cover undo boundary conditions`

Keep unrelated changes in separate commits when practical.

## 4. C coding expectations

- Prefer standard C11 features unless a platform-specific dependency is justified.
- Include the headers required by the functions being used.
- Keep functions focused and name them according to their behavior.
- Validate all indexes before using them to access arrays.
- Check the return value of input functions such as `fgets`.
- Ensure strings are terminated and destination capacity is respected.
- Avoid unchecked `strcpy`, `strcat`, and assumptions that `strncpy` always adds a null terminator.
- Check the resulting length before performing replacement or concatenation.
- Handle empty search strings explicitly to avoid invalid or non-progressing replacement logic.
- Make state changes atomic where possible: validate first, then record history, then mutate.
- Keep undo/redo behavior consistent for every mutating operation.
- Do not silently discard user text when a buffer is full.
- Keep terminal control sequences separate from the underlying text data.
- Avoid global state when a clear state structure or function parameter would make ownership easier to reason about.
- Add comments for non-obvious invariants, not for every line.

When changing a capacity constant, review every dependent array, parser, copy operation, display loop, and documentation reference.

## 5. Behavior and compatibility

A pull request that changes command syntax or editing behavior must update:
- README command summary.
- [Command reference](docs/COMMAND-REFERENCE.md).
- Relevant architecture or data-model documentation.
- Bug and feature issue templates if reporting requirements change.
- Manual regression steps or automated tests.

Do not claim support for multi-word replacement arguments, persistent files, independent per-buffer undo stacks, or syntax-aware indentation until those behaviors exist and are tested.

## 6. Testing expectations

At minimum, compile with warnings enabled:

```sh
gcc -std=c11 -Wall -Wextra -pedantic "TEXT EDITOR/main.c" -o build/text-editor
```

For interactive changes, run the program and test the affected command. Include boundary cases such as:
- Empty input and end-of-file.
- A line at the maximum supported length and a line that exceeds it.
- The first and last valid line numbers.
- Invalid line numbers such as zero and a number greater than the line count.
- An empty buffer and a full buffer.
- Undo with no history and redo with no redo history.
- Repeated undo and redo operations.
- Switching between buffers after editing each one.
- Replacement with no match, multiple matches, an empty search term, and a replacement longer than the match.
- Text containing braces inside strings or comments, if indentation logic is changed.

If a test is not automated, state that it was performed manually. Never report a test as passed if it was not run.

## 7. Documentation expectations

Documentation is part of the feature. Update it when behavior, commands, limits, build steps, or known risks change. Keep statements factual and distinguish:
- Implemented behavior.
- Known defects or limitations.
- Proposed future work.

Use relative links for files inside this repository. Include complete commands that can be copied from the repository root.

## 8. Pull request checklist

Before opening a pull request:
- [ ] The change has one clear purpose.
- [ ] The implementation validates indexes and input lengths.
- [ ] String operations respect destination capacities.
- [ ] Undo/redo behavior is correct for the changed operation.
- [ ] Buffer switching does not create unexpected state leakage.
- [ ] The project compiles with the documented command.
- [ ] Relevant manual or automated tests were run.
- [ ] Documentation and examples match the implementation.
- [ ] No generated executable, temporary file, or local IDE setting was added.
- [ ] No credentials, private data, or unrelated files are included.
- [ ] The pull request description explains risk and compatibility impact.

## 9. Review process

Maintainers may request a smaller scope, additional tests, clearer documentation, or changes to unsafe code paths. A review request is not a personal criticism. Keep discussion technical and respectful.

Changes may be declined when they introduce unbounded memory operations, hide failures, misrepresent unsupported behavior, or expand the project beyond the agreed scope without a clear design.

## 10. Reporting a problem

Use the GitHub bug report template. Include compiler and operating system details, the exact command sequence, expected behavior, actual behavior, and the smallest reproducible example. Do not include private information or potentially harmful payloads in a public issue.

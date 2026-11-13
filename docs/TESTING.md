# Testing Strategy

## Current automated coverage

The GitHub Actions workflow currently checks that `TEXT EDITOR/main.c` compiles with GCC on Ubuntu. This is a build check, not a behavioral test suite.

There are no dedicated unit tests or automated interactive tests in the current repository. A green build means the compiler accepted the source with the configured flags. It does not establish that undo, redo, replacement, or boundary conditions work correctly.

## Compile check

```sh
gcc -std=c11 -Wall -Wextra -pedantic "TEXT EDITOR/main.c" -o build/text-editor
```

## Manual smoke test

Launch the program and try the following sequence:

1. Enter two ordinary text lines.
2. Run `edit 1 Updated first line`.
3. Run `undo` and verify the previous state is restored.
4. Run `redo` and verify the edited state returns.
5. Run `replace Updated Revised` and inspect every line.
6. Run `switch 2`, add text, then return with `switch 1`.
7. Run `quit` and confirm the process exits.

This sequence is a basic smoke test only. It does not cover all defects or prove the behavior is safe.

## Boundary test matrix

| Area | Scenario | Expected safe behavior |
|---|---|---|
| Input | Empty line | Accepted or rejected consistently, without stale data |
| Input | End-of-file | Program exits cleanly |
| Input | Input longer than the input array | Truncation is detected and handled |
| Line storage | Add line 100 | Accepted if capacity remains |
| Line storage | Add line 101 | Clear capacity message, no out-of-bounds write |
| Line length | 255 characters | Correctly stored with termination |
| Line length | More than 255 characters | Rejected or safely truncated with explicit feedback |
| Edit | First line | Correctly updates line 1 |
| Edit | Last existing line | Correctly updates final line |
| Edit | Line 0 | Rejected without changing history |
| Edit | Line greater than count | Rejected without changing history |
| Undo | No prior mutation | Clear no-history response |
| Redo | No prior undo | Clear no-redo response |
| History | More than 50 edits | Defined overflow policy; no invalid stack index |
| Replace | No match | Text remains unchanged |
| Replace | Multiple matches in one line | Behavior is documented and consistent |
| Replace | Empty search text | Rejected safely |
| Replace | Longer replacement | No overflow |
| Buffers | Edit two buffers and undo | History applies to the correct buffer |
| Indentation | Switch after brace changes | Indentation is buffer-specific or deliberately documented |
| Rendering | Text with punctuation | Display does not drop or reorder characters |

## Regression test priorities

The first automated tests should target pure functions after input, rendering, and state management are separated from the main loop. Good initial candidates:
- Validate a line number.
- Append a line within capacity.
- Reject append when full.
- Replace a line without exceeding its capacity.
- Apply a replacement with exact output length checks.
- Save, undo, and redo a state.
- Confirm a failed operation does not mutate state or history.

## Sanitizer testing

Compile with AddressSanitizer and UndefinedBehaviorSanitizer where the compiler supports them:

```sh
gcc -std=c11 -Wall -Wextra -pedantic -g -fsanitize=address,undefined "TEXT EDITOR/main.c" -o build/text-editor-sanitized
```

Then exercise long input and replacement operations in a disposable environment. Do not treat the absence of a sanitizer report in a short run as proof of memory safety.

## CI improvement path

A practical sequence is:
1. Keep the current compile job.
2. Extract state operations into testable functions.
3. Add a small test executable with deterministic assertions.
4. Run tests in CI.
5. Add sanitizer builds on supported runners.
6. Add Windows compilation after platform-specific terminal behavior is isolated.

Do not add a test job that merely prints success without executing assertions.

# Data Model, Buffers, and History

## EditorState

The editor's primary data structure is:

```c
typedef struct {
    char text[MAX_LINES][MAX_LINE_LENGTH];
    int line_count;
} EditorState;
```

The source currently defines:
- `MAX_LINES = 100`
- `MAX_LINE_LENGTH = 256`
- `MAX_HISTORY = 50`
- `MAX_FILES = 5`

An `EditorState` contains storage for every possible line, not just the active lines. The `line_count` field indicates how many rows should be displayed and considered part of the current document.

## Buffer storage

The global `files` array contains five `EditorState` values. `current_file` selects the active slot. In the main function, a pointer named `state` points to the selected entry.

This is not a filesystem abstraction. The name `files` is historical terminology in the code. A buffer is an in-memory text area and is discarded when the program exits.

## History representation

Undo and redo store complete copies of `EditorState`. Since the structure contains a fixed array, structure assignment copies the entire array and the line count. This approach is easy to understand but consumes more memory than storing individual edit operations.

The two stacks are global:
- `undo_stack` stores previous snapshots.
- `redo_stack` stores states that can be restored after undo.
- `undo_top` and `redo_top` track the most recently used slots.

When `save_to_undo` succeeds, it clears the redo stack. When the undo stack is full, the current implementation does not shift out the oldest entry or report that history is full. It simply stops recording new snapshots.

## State consistency concerns

### History is not per buffer

Switching from buffer 1 to buffer 2 does not select a separate undo history. A later undo operates on the state pointer passed by the caller while consuming the global history stack. This can restore a snapshot created while another buffer was active.

A robust design should store history inside each buffer or associate every snapshot with a buffer identifier.

### Indentation is not part of the snapshot

The `indent_level` global is not inside `EditorState`. Undoing text does not undo indentation changes. Switching buffers also does not restore a buffer-specific indentation level.

### Mutations do not all use history

Adding and editing lines call `save_to_undo`, but find-and-replace does not. This makes undo behavior inconsistent.

### Invalid edits can create snapshots

The edit function records history before checking the line number. Validation should happen before history is changed.

## Recommended future model

A maintainable next step would be to make buffer ownership explicit:

```c
typedef struct {
    EditorState current;
    EditorState undo[MAX_HISTORY];
    EditorState redo[MAX_HISTORY];
    size_t undo_count;
    size_t redo_count;
    int indent_level;
} EditorBuffer;
```

This is an illustrative design direction, not code currently present in the repository. A production implementation should also define what happens when history reaches capacity, centralize all mutations, and avoid copying uninitialized or irrelevant bytes.

## Memory sizing

With the current constants, the text matrix in one state reserves 100 × 256 bytes, or 25,600 bytes, before accounting for the line count and structure padding. The five active buffers therefore reserve approximately 128 KB for text. Each history array reserves space for 50 complete states, so undo and redo together reserve approximately 2.56 MB for text snapshots, excluding small structure overhead.

This is static storage, not memory allocated dynamically while editing. Changing constants changes these estimates and should be reflected in this document.

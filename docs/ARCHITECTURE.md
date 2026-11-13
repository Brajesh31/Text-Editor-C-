# Architecture

The program is a single-process terminal application implemented in `TEXT EDITOR/main.c`.

## Main data structures

- `EditorState`: fixed-size array of lines and a line count.
- `files`: five in-memory editor states.
- `undo_stack` and `redo_stack`: snapshots of editor state.
- `current_file`: index of the active buffer.

## Main flow

```mermaid
flowchart TD
    A[main] --> B[Initialize current buffer]
    B --> C[display_text]
    C --> D[fgets command input]
    D --> E[Parse command]
    E --> F[add_line / edit_line]
    E --> G[undo / redo]
    E --> H[find_and_replace]
    E --> I[switch_file]
    F --> C
    G --> C
    H --> C
    I --> C
    E -->|quit| J[Return from main]
```

There is no filesystem persistence layer. The editor state exists only in process memory and is discarded at exit.

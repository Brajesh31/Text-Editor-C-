#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 100
#define MAX_LINE_LENGTH 256
#define MAX_HISTORY 50
#define MAX_FILES 5

typedef struct {
    char text[MAX_LINES][MAX_LINE_LENGTH];
    int line_count;
} EditorState;

// Undo/Redo stacks
EditorState undo_stack[MAX_HISTORY];
EditorState redo_stack[MAX_HISTORY];
int undo_top = -1, redo_top = -1;

// Files and current file index
EditorState files[MAX_FILES];
int current_file = 0;

// Indentation level
int indent_level = 0;

// C keywords for highlighting
const char *keywords[] = {"int", "char", "return", "if", "else", "for", "while"};
int keyword_count = sizeof(keywords) / sizeof(keywords[0]);

// Save current state to undo stack
void save_to_undo(EditorState *current_state) {
    if (undo_top < MAX_HISTORY - 1) {
        undo_stack[++undo_top] = *current_state;
        redo_top = -1; // Clear redo stack
    }
}

// Restore state from undo stack
int undo(EditorState *current_state) {
    if (undo_top >= 0) {
        redo_stack[++redo_top] = *current_state; // Save to redo stack
        *current_state = undo_stack[undo_top--];
        return 1;
    }
    printf("Nothing to undo!\n");
    return 0;
}

// Restore state from redo stack
int redo(EditorState *current_state) {
    if (redo_top >= 0) {
        undo_stack[++undo_top] = *current_state; // Save to undo stack
        *current_state = redo_stack[redo_top--];
        return 1;
    }
    printf("Nothing to redo!\n");
    return 0;
}

// Check if a word is a keyword
int is_keyword(const char *word) {
    for (int i = 0; i < keyword_count; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Highlight a single line
void highlight_text(const char *line) {
    const char *delimiters = " \t\n,;(){}[]";
    char temp[MAX_LINE_LENGTH];
    strcpy(temp, line);
    char *token = strtok(temp, delimiters);

    while (token != NULL) {
        if (is_keyword(token)) {
            printf("\033[1;32m%s\033[0m", token); // Highlight keywords in green
        } else {
            printf("%s", token);
        }
        // Print the delimiter between tokens
        const char *next_token = token + strlen(token);
        while (*next_token && strchr(delimiters, *next_token)) {
            putchar(*next_token);
            next_token++;
        }
        token = strtok(NULL, delimiters);
    }
    printf("\n");
}

// Display text with indentation and highlighting
void display_text(EditorState *state) {
#ifdef _WIN32
    system("cls"); // Clear the screen for Windows
#else
    system("clear"); // Clear the screen for Unix/Linux
#endif
    printf("Multi-Feature Text Editor\n");
    for (int i = 0; i < state->line_count; i++) {
        for (int j = 0; j < indent_level; j++) printf("\t");
        highlight_text(state->text[i]);
    }
    printf("\n> ");
}

// Add a line with auto-indentation
void add_line(EditorState *state, const char *line) {
    if (state->line_count < MAX_LINES) {
        save_to_undo(state);
        if (strchr(line, '{')) indent_level++;
        else if (strchr(line, '}') && indent_level > 0) indent_level--;
        strncpy(state->text[state->line_count++], line, MAX_LINE_LENGTH);
    }
}

// Edit a specific line
void edit_line(EditorState *state, int line_number, const char *new_text) {
    save_to_undo(state);
    if (line_number < 1 || line_number > state->line_count) {
        printf("Invalid line number.\n");
        return;
    }
    strncpy(state->text[line_number - 1], new_text, MAX_LINE_LENGTH);
}

// Switch between files
void switch_file(int file_index) {
    if (file_index < 0 || file_index >= MAX_FILES) {
        printf("Invalid file index. Max files: %d\n", MAX_FILES);
        return;
    }
    current_file = file_index;
    printf("Switched to file %d.\n", current_file + 1);
}

// Find and replace text
void find_and_replace(EditorState *state, const char *find, const char *replace) {
    for (int i = 0; i < state->line_count; i++) {
        char *pos = strstr(state->text[i], find);
        if (pos) {
            char buffer[MAX_LINE_LENGTH];
            strncpy(buffer, state->text[i], pos - state->text[i]);
            buffer[pos - state->text[i]] = '\0';
            strcat(buffer, replace);
            strcat(buffer, pos + strlen(find));
            strncpy(state->text[i], buffer, MAX_LINE_LENGTH);
        }
    }
}

int main() {
    EditorState *state = &files[current_file];
    char input[MAX_LINE_LENGTH];
    char command[16], argument[MAX_LINE_LENGTH];
    int line_number;

    while (1) {
        display_text(state);
        fgets(input, sizeof(input), stdin);

        if (strcmp(input, "undo\n") == 0) {
            undo(state);
        } else if (strcmp(input, "redo\n") == 0) {
            redo(state);
        } else if (sscanf(input, "edit %d %[^\n]", &line_number, argument) == 2) {
            edit_line(state, line_number, argument);
        } else if (sscanf(input, "replace %s %s", command, argument) == 2) {
            find_and_replace(state, command, argument);
        } else if (sscanf(input, "switch %d", &line_number) == 1) {
            switch_file(line_number - 1);
            state = &files[current_file];
        } else if (strcmp(input, "quit\n") == 0) {
            break;
        } else {
            add_line(state, input);
        }
    }

    return 0;
}
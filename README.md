# Todo TUI

A simple terminal-based Todo list application written in C using the ncurses library. This application provides a clean and efficient way to manage your tasks directly from the terminal.

## Features

- Add, delete, and mark tasks as done/undone
- Navigate tasks using arrow keys or vim-style keys (j/k)
- Clean and minimal interface
- Non-blocking input for smooth interaction
- Persistent task list during session

## Requirements

- C compiler (gcc recommended)
- ncurses library
- Zig compiler (optional)
  - Can be used to build the project with better error handling and build system
  - Provides cross-compilation capabilities
  - Offers memory safety features
  - Can be used to gradually migrate the codebase to Zig

## Building

To build the project, simply run:

```bash
gcc -o todo main.c ui.c tasks.c -lncurses
```

Or using Zig:

```bash
zig cc -o todo main.c ui.c tasks.c -lncurses
```

## Usage

Run the compiled binary:

```bash
./todo
```

### Controls

- `j` or `↓`: Move selection down
- `k` or `↑`: Move selection up

## Project Structure

- `main.c`: Main program loop and initialization
- `ui.c`: User interface handling and drawing
- `tasks.c`: Task management functions
- `include/ui.h`: UI function declarations
- `include/tasks.h`: Task management function declarations
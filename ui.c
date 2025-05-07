#include <ncurses.h>
#include <string.h>
#include "include/ui.h"
#include "include/tasks.h"

void init_ui() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);  // Enable non-blocking input
    curs_set(0);  // Hide the cursor
}

void end_ui() {
    endwin();
}

int get_input() {
    int ch = getch();
    if (ch == ERR) return 0;  // Return 0 when no input is available
    return ch;
}

void draw_ui() {
    clear();
    mvprintw(0, 0, "TODO List, [d]one, [a]dd, [x]delete, [q]uit");

    if (task_count == 0) {
        mvprintw(2, 2, "No tasks yet. Press 'a' to add one!");
    } else {
        for (int i = 0; i < task_count; i++) {
            if (i == selected_index) attron(A_REVERSE);
            mvprintw(i + 2, 2, "- %s", tasks[i]);
            if (i == selected_index) attroff(A_REVERSE);
        }
    }

    refresh();
}

void handle_input(int ch) {
    switch (ch) {
        case KEY_UP:
        case 'k':
            if (selected_index > 0) selected_index--;
            break;
        case KEY_DOWN:
        case 'j':
            if (selected_index < task_count - 1) selected_index++;
            break;
        case 'x':
            delete_task(selected_index);
            break;
        case 'a':
            nodelay(stdscr, FALSE);  // Temporarily disable non-blocking mode
            curs_set(1);  // Show cursor for input
            echo();
            char buffer[256];
            mvprintw(task_count + 4, 2, "New Task: ");
            getnstr(buffer, 255);
            noecho();
            curs_set(0);  // Hide cursor again
            nodelay(stdscr, TRUE);   // Re-enable non-blocking mode
            if (strlen(buffer) > 0) {  // Only add if there's actual input
                add_task(buffer);
            }
            break;
        case 'd':
            toggle_done(selected_index);
            break;
    }
}

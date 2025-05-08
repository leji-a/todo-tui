#include <ncurses.h>
#include <string.h>
#include "include/ui.h"
#include "include/tasks.h"

// Add column selection tracking
static int selected_column = 0;  // 0 for active tasks, 1 for completed tasks

// Helper function to get task index in current column
static int get_column_task_index(int global_index) {
    int count = 0;
    for (int i = 0; i < task_count; i++) {
        if ((selected_column == 0 && tasks[i][0] != '[') ||
            (selected_column == 1 && tasks[i][0] == '[')) {
            if (count == global_index) return i;
            count++;
        }
    }
    return -1;
}

// Helper function to get total tasks in current column
static int get_column_task_count() {
    int count = 0;
    for (int i = 0; i < task_count; i++) {
        if ((selected_column == 0 && tasks[i][0] != '[') ||
            (selected_column == 1 && tasks[i][0] == '[')) {
            count++;
        }
    }
    return count;
}

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

    int start_y = 1;
    int mid_x = COLS / 2;
    int height = task_count + 2;  // Add padding
    int left_width = mid_x - 2;
    int right_width = COLS - mid_x - 2;

    // Draw headers
    attron(A_BOLD);
    mvprintw(start_y, 2, "ACTIVE TASKS");
    mvprintw(start_y, mid_x + 2, "COMPLETED TASKS");
    attroff(A_BOLD);

    // Draw left border (Active Tasks)
    mvaddch(start_y + 1, 0, ACS_ULCORNER);
    mvaddch(start_y + 1, left_width, ACS_URCORNER);
    mvaddch(start_y + height, 0, ACS_LLCORNER);
    mvaddch(start_y + height, left_width, ACS_LRCORNER);

    // Draw right border (Completed Tasks)
    mvaddch(start_y + 1, mid_x, ACS_ULCORNER);
    mvaddch(start_y + 1, COLS - 2, ACS_URCORNER);
    mvaddch(start_y + height, mid_x, ACS_LLCORNER);
    mvaddch(start_y + height, COLS - 2, ACS_LRCORNER);

    // Draw horizontal lines
    for (int x = 1; x < left_width; x++) {
        mvaddch(start_y + 1, x, ACS_HLINE);
        mvaddch(start_y + height, x, ACS_HLINE);
    }
    for (int x = mid_x + 1; x < COLS - 2; x++) {
        mvaddch(start_y + 1, x, ACS_HLINE);
        mvaddch(start_y + height, x, ACS_HLINE);
    }

    // Draw vertical lines
    for (int y = 2; y < height; y++) {
        mvaddch(start_y + y, 0, ACS_VLINE);
        mvaddch(start_y + y, left_width, ACS_VLINE);
        mvaddch(start_y + y, mid_x, ACS_VLINE);
        mvaddch(start_y + y, COLS - 2, ACS_VLINE);
    }

    // Draw tasks
    int active_count = 0;
    int done_count = 0;
    
    for (int i = 0; i < task_count; i++) {
        if (tasks[i][0] == '[') {
            // Completed task - draw on right side
            if (selected_column == 1 && done_count == selected_index) attron(A_REVERSE);
            mvprintw(done_count + 3, mid_x + 2, "- %s", tasks[i]);
            if (selected_column == 1 && done_count == selected_index) attroff(A_REVERSE);
            done_count++;
        } else {
            // Active task - draw on left side
            if (selected_column == 0 && active_count == selected_index) attron(A_REVERSE);
            mvprintw(active_count + 3, 2, "- %s", tasks[i]);
            if (selected_column == 0 && active_count == selected_index) attroff(A_REVERSE);
            active_count++;
        }
    }

    if (task_count == 0) {
        mvprintw(3, 2, "No tasks yet. Press 'a' to add one!");
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
            if (selected_index < get_column_task_count() - 1) selected_index++;
            break;
        case KEY_LEFT:
        case 'h':
            if (selected_column > 0) {
                selected_column = 0;
                selected_index = 0;
            }
            break;
        case KEY_RIGHT:
        case 'l':
            if (selected_column < 1) {
                selected_column = 1;
                selected_index = 0;
            }
            break;
        case 'x':
            {
                int actual_index = get_column_task_index(selected_index);
                if (actual_index != -1) {
                    delete_task(actual_index);
                    if (selected_index >= get_column_task_count()) {
                        selected_index = get_column_task_count() - 1;
                    }
                }
            }
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
            {
                int actual_index = get_column_task_index(selected_index);
                if (actual_index != -1) {
                    toggle_done(actual_index);
                    // Move selection to the other column if task was toggled
                    selected_column = !selected_column;
                    selected_index = 0;
                }
            }
            break;
    }
}

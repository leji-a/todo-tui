#include <ncurses.h>
#include <unistd.h>  // for usleep
#include "include/ui.h"
#include "include/tasks.h"

#define TASKS_FILE "tasks.txt"

int main() {
    init_ui();
    
    // Try to load tasks from file, if it fails, load sample tasks
    if (!load_tasks_from_file(TASKS_FILE)) {
        load_sample_tasks();
    }

    int ch;
    while ((ch = get_input()) != 'q') {
        if (ch != 0) {  // Only handle input if there is any
            handle_input(ch);
        }
        draw_ui();
        usleep(100000);  // Sleep for 100ms to prevent high CPU usage
    }
    
    cleanup_tasks();
    end_ui();
    return 0;
}
#include <ncurses.h>
#include <unistd.h>  // for usleep
#include "include/ui.h"
#include "include/tasks.h"

int main() {
    init_ui();
    load_sample_tasks();  // Optional, just for demo

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
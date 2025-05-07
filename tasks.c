#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "include/tasks.h"

char *tasks[MAX_TASKS];
int task_count = 0;
int selected_index = 0;

void add_task(const char *task) {
    if (task_count < MAX_TASKS) {
        tasks[task_count] = strdup(task);
        task_count++;
    }
}

void delete_task(int index) {
    if (index < 0 || index >= task_count) return;
    free(tasks[index]);
    for (int i = index; i < task_count - 1; i++) {
        tasks[i] = tasks[i + 1];
    }
    task_count--;
    if (selected_index >= task_count) selected_index = task_count - 1;
}

void toggle_done(int index) {
    if (index < 0 || index >= task_count) return;
    
    if (tasks[index][0] == '[') {
        // Task is done, mark as not done
        char *new_task = malloc(MAX_LEN);
        strcpy(new_task, tasks[index] + 7);  // Skip "[DONE] "
        free(tasks[index]);
        tasks[index] = new_task;
    } else {
        // Task is not done, mark as done
        char *new_task = malloc(MAX_LEN);
        snprintf(new_task, MAX_LEN, "[DONE] %s", tasks[index]);
        free(tasks[index]);
        tasks[index] = new_task;
    }
}

void load_sample_tasks() {
    add_task("Buy milk");
    add_task("Write modular TUI in C");
}

void cleanup_tasks() {
    for (int i = 0; i < task_count; i++) free(tasks[i]);
}

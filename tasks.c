#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "include/tasks.h"

char *tasks[MAX_TASKS];
int task_count = 0;
int selected_index = 0;

// Helper function to save tasks after modifications
static void save_after_change() {
    save_tasks_to_file("tasks.txt");
}

void add_task(const char *task) {
    if (task_count < MAX_TASKS) {
        tasks[task_count] = strdup(task);
        task_count++;
        save_after_change();
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
    save_after_change();
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
    save_after_change();
}

void load_sample_tasks() {
    add_task("santi gay");
    add_task("santi boludoo");
}

void cleanup_tasks() {
    for (int i = 0; i < task_count; i++) free(tasks[i]);
}

int save_tasks_to_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        return 0;  // Failed to open file
    }

    for (int i = 0; i < task_count; i++) {
        fprintf(file, "%s\n", tasks[i]);
    }

    fclose(file);
    return 1;  // Success
}

int load_tasks_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return 0;  // Failed to open file
    }

    // Clear existing tasks
    cleanup_tasks();
    task_count = 0;

    char buffer[MAX_LEN];
    while (task_count < MAX_TASKS && fgets(buffer, MAX_LEN, file)) {
        // Remove newline if present
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        add_task(buffer);
    }

    fclose(file);
    return 1;  // Success
}

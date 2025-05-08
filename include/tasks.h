#ifndef TASKS_H
#define TASKS_H

#define MAX_TASKS 100
#define MAX_LEN 256

extern char *tasks[MAX_TASKS];
extern int task_count;
extern int selected_index;

void add_task(const char *task);
void delete_task(int index);
void toggle_done(int index);
void load_sample_tasks();
void cleanup_tasks();
int save_tasks_to_file(const char *filename);
int load_tasks_from_file(const char *filename);

#endif

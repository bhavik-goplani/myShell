#include "main.h"

#define MAX_JOBS 100

typedef struct {
    int job_id;
    pid_t pid;
    char command[100]; // Adjust the size as needed
    int completed;     // 0 for running, 1 for completed
} Job;

Job jobs[MAX_JOBS]; // Store job information in this array
int job_count = 0; // Keep track of the number of jobs

void add_job(pid_t pid, const char *command) {
    if (job_count < MAX_JOBS) {
        jobs[job_count].job_id = job_count + 1;
        jobs[job_count].pid = pid;
        strncpy(jobs[job_count].command, command, sizeof(jobs[job_count].command));
        jobs[job_count].completed = 0; // Job is running
        job_count++;
    }
}

void print_jobs() {
    int i;
    for (i = 0; i < job_count; i++) {
        if (jobs[i].completed == 0) {
            printf("[%d] %s\n", jobs[i].job_id, jobs[i].command);
        }
        else {
            printf("[%d] %s (completed)\n", jobs[i].job_id, jobs[i].command);
        }
    }
}
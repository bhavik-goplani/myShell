#include "main.h"

#define MAX_JOBS 100

// Structure to hold background jobs
typedef struct {
    pid_t pid;
    int job_id;
    int completed;

    char command[256];
} Job;

Job jobs[256]; // Array to hold background jobs
int job_count = 0;

// Function to add a job to the jobs array
void add_job(pid_t pid, const char *command) {
    jobs[job_count].pid = pid;
    jobs[job_count].job_id = job_count + 1;
    strncpy(jobs[job_count].command, command, 255);
    job_count++;
}

// Function to remove a job from the jobs array
void remove_job(pid_t pid) {
    for (int i = 0; i < job_count; i++) {
        if (jobs[i].pid == pid) {
            for (int j = i; j < job_count - 1; j++) {
                jobs[j] = jobs[j + 1];
                jobs[j].job_id--;
            }
            print_jobs();
            job_count--;
            break;
        }
    }
}

// Function to check for completed background jobs
void check_jobs() {
    for (int i = 0; i < job_count; i++) {
        if (waitpid(jobs[i].pid, NULL, WNOHANG) > 0) {
            printf("Completed: [%d] %d %s\n", jobs[i].job_id, jobs[i].pid, jobs[i].command);
            jobs[i].completed = 1;
            remove_job(jobs[i].pid);
            i--; // Adjust index after removing a job
        }
    }
}

void print_jobs() {
    for (int i = 0; i < job_count; i++) {
        if (jobs[i].completed == 0) {
            printf("[%d] %d %s &\n", jobs[i].job_id, jobs[i].pid, jobs[i].command);
        }
    }
}

void handle_kill_command(int signum, pid_t pid) {
    if (kill(pid, signum) == -1) {
        perror("kill");
    } else {
        // If the signal is SIGTERM or SIGINT, remove the job
        // printf("Signal %d sent to PID %d\n", signum, pid);
        remove_job(pid);
    }
}


pid_t get_pid_from_jobid(int job_id) {
    for (int i = 0; i < job_count; i++) {
        if (jobs[i].job_id == job_id) {
            return jobs[i].pid;
        }
    }
    return -1; // Return -1 if jobid not found
}
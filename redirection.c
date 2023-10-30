#include "main.h"

int redirection_check(char **argv) {
    int i = 0;
    while (argv[i] != NULL) {
        if (strcmp(argv[i], ">>") == 0) {
            return 4;
        } else if (strcmp(argv[i], ">") == 0) {
            return 2;
        } else if (strcmp(argv[i], "<") == 0) {
            return 1;
        }
        i++;
    }
    return -1;
}

int handle_redirection(char **argv, int state, int *index) {
    int fd;
    char *filename = argv[*index + 1];
    if (filename == NULL) {
        fprintf(stderr, "Error: No filename provided for redirection.\n");
        return -1;
    }

    switch (state) {
        case 1: // Input redirection
            if ((fd = open(filename, O_RDONLY, 0644)) < 0) {
                perror("open error");
                return -1;
            }
            dup2(fd, 0);
            close(fd);
            break;

        case 2: // Output redirection
            if ((fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644)) < 0) {
                perror("open error");
                return -1;
            }
            dup2(fd, 1);
            close(fd);
            break;

        case 4: // Appending output redirection
            if ((fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644)) < 0) {
                perror("open error");
                return -1;
            }
            dup2(fd, 1);
            close(fd);
            break;

        default:
            fprintf(stderr, "Error: Invalid redirection type.\n");
            return -1;
    }

    argv[*index] = NULL; // Terminate the argv array here
    *index += 2; // Skip the redirection symbol and filename
    return 0;
}

int redirection(char **argv) {
    int i = 0;
    while (argv[i] != NULL) {
        int state = -1;
        if (strcmp(argv[i], "<") == 0) {
            state = 1;
        } else if (strcmp(argv[i], ">") == 0) {
            state = 2;
        } else if (strcmp(argv[i], ">>") == 0) {
            state = 4;
        }

        if (state != -1) {
            if (handle_redirection(argv, state, &i) == -1) {
                return -1;
            }
        } else {
            i++;
        }
    }
    execcmd(argv);
    perror("execvp error");
    _exit(EXIT_FAILURE);
}
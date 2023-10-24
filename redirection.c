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

int redirection_count(char **argv) {
    int i = 0;
    int count = 0;
    while (argv[i] != NULL) {
        if (strcmp(argv[i], ">>") == 0) {
            count++;
        } else if (strcmp(argv[i], ">") == 0) {
            count++;
        } else if (strcmp(argv[i], "<") == 0) {
            count++;
        }
        i++;
    }

    return count;
}

int* redirection_operators(char **argv) {
    int i = 0;
    int count = redirection_count(argv);
    int *operators = malloc(sizeof(int) * count);
    int j = 0;
    while (argv[i] != NULL) {
        if (strcmp(argv[i], ">>") == 0) {
            operators[j] = 4;
            j++;
        } else if (strcmp(argv[i], ">") == 0) {
            operators[j] = 2;
            j++;
        } else if (strcmp(argv[i], "<") == 0) {
            operators[j] = 1;
            j++;
        }
        i++;
    }

    return operators;
}

int redirection(char **argv, int state) {
    int i = 0, fd;
    char *filename = NULL;

    // Find the redirection symbol and filename
    while (argv[i] != NULL) {
        if ((state == 1 && strcmp(argv[i], "<") == 0) ||
            (state == 2 && strcmp(argv[i], ">") == 0) ||
            (state == 4 && strcmp(argv[i], ">>") == 0)) {
            filename = argv[i + 1];
            argv[i] = NULL; // Terminate the argv array here
            break;
        }
        i++;
    }

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

    execvp(argv[0], argv);
    perror("execvp error");
    _exit(EXIT_FAILURE);
}

int multiple_redirection(char **argv, int* state_counts){

    int i = 0, j = 0 fd;
    char *filename = NULL;
    while (argv[i] != NULL) {
        if ((state == 1 && strcmp(argv[i], "<") == 0) ||
            (state == 2 && strcmp(argv[i], ">") == 0) ||
            (state == 4 && strcmp(argv[i], ">>") == 0)) {
            filename = argv[i + 1];
            argv[i] = NULL; // Terminate the argv array here
            break;
        }
        i++;
    
    }

    
}

#include "main.h"

int main(int argc, char **argv) {
    char *prompt = "[QUASH]$ ";
    char *lineptr;
    size_t n = 0;
    ssize_t read = 0;
    pid_t pid;

    (void)argc;
    printf("Welcome...\n");

    while (1) {
        lineptr = NULL;
        printf("%s", prompt);
        read = getline(&lineptr, &n, stdin);
        
        if (read == -1) {
            free(lineptr);    
            return (0);
        }
        
        argv = parse_command(lineptr, argv);

        pid = fork();
        if (pid == 0) {
            execcmd(argv);
            exit(0);
        }
        else {
            wait(NULL);
        }

        free_argv(argv);
    }
    return (0);
}

static void free_argv(char **argv) {
    for (int i = 0; argv[i] != NULL; i++) {
        free(argv[i]);
        argv[i] = NULL;
    }
    free(argv);
    argv = NULL;
}
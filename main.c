#include "main.h"

int main(int argc, char **argv) {
    char *prompt = "[QUASH]$ ";
    char *lineptr;
    size_t n = 0;
    ssize_t read = 0;

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

        remove_comments(lineptr);
        
        argv = parse_command(lineptr, argv);
        if (sh_execute(argv) == -1) {
            free_argv(argv);
            return (0);
        }
        free_argv(argv);
    }
    return (0);
}

void free_argv(char **argv) {
    for (int i = 0; argv[i] != NULL; i++) {
        if (argv[i] != NULL){
            free(argv[i]);
            argv[i] = NULL;
        }
    }
    free(argv);
    argv = NULL;
}

int sh_execute(char **argv) {
    int i;
    int redirection_type;

    if (argv[0] == NULL) {
        return (1);
    }

    // Check for pipes before checking for built-ins
    for (int i = 0; argv[i] != NULL; i++) {
        if (strcmp(argv[i], "|") == 0) {
            if (argv[1] == NULL || strcmp(argv[1], "|") == 0) {
                fprintf(stderr, "Syntax error: Invalid use of pipe\n");
                return 1;
            }
            return sh_pipe(argv);
        }
    }

    redirection_type = redirection_check(argv);
    if (redirection_type != -1) {
       return execute_redirection(argv);
    }

    for (i = 0; i < sh_num_builtins(); i++) {
        if (strcmp(argv[0], builtin_str[i]) == 0) {
            return ((*builtin_func[i])(argv));
        }
    }
    return (sh_launch(argv));
}

int sh_launch(char **argv) {
    pid_t pid;

    pid = fork();
    if (pid == 0) {
        execcmd(argv);
        exit(0);
    }
    else if (pid < 0) {
        perror("ErrorForking:");
    }
    else {
        wait(NULL);
    }

    return (1);
}

int execute_redirection(char **argv) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) { // Child process
        redirection(argv);
        exit(EXIT_FAILURE); // If redirection fails, exit child
    } else if (pid < 0) { // Forking error
        perror("ErrorForking:");
        return -1;
    } else { // Parent process
        wait(&status); // Wait for child to finish
    }

    return 1;
}
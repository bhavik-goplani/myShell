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
        free(argv[i]);
        argv[i] = NULL;
    }
    free(argv);
    argv = NULL;
}

int sh_execute(char **argv) {
    int i;

    if (argv[0] == NULL) {
        return (1);
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
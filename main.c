#include "main.h"

int main(int argc, char **argv) {
    char *prompt = "[QUASH]$ ";
    char *lineptr = NULL, *lineptr_copy = NULL;
    const char *delimiter = " \n";
    char *token = NULL;
    size_t n = 0;
    ssize_t read = 0;
    int num_tokens = 0;

    (void)argc;
    printf("Welcome...\n");

    while (1) {
        num_tokens = 0;
        printf("%s", prompt);
        read = getline(&lineptr, &n, stdin);
        
        if (read == -1) {
            free(lineptr);    
            return (0);
        }
        
        lineptr_copy = strdup(lineptr);

        token = strtok(lineptr, delimiter);
        
        while (token != NULL) {
            num_tokens++;
            token = strtok(NULL, delimiter);
        }
        argv = malloc(sizeof(char *) * (num_tokens+1));

        num_tokens = 0;
        token = strtok(lineptr_copy, delimiter);
        while (token != NULL) {
            argv[num_tokens] = strdup(token);
            num_tokens++;
            token = strtok(NULL, delimiter);
        }
        argv[num_tokens] = NULL;

        execcmd(argv);

        free(lineptr);
        lineptr = NULL;
        free(lineptr_copy);
        lineptr_copy = NULL;
        for (int i = 0; argv[i] != NULL; i++) {
            free(argv[i]);
            argv[i] = NULL;
        }
        free(argv);
        argv = NULL;
    }
    return (0);
}

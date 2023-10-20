#include "main.h"

char **parse_command(char *lineptr, char **argv) {
    char *lineptr_copy = NULL, *token = NULL;
    const char *delimiter = " \n";
    int num_tokens = 0;

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

    free(lineptr);
    free(lineptr_copy);

    return (argv);
}
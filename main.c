#include "main.h"

int main(int argc, char **argv) {
    char *prompt = "[QUASH]$ ";
    char *lineptr = NULL, *lineptr_copy = NULL;
    const char *delimiter = " \n";
    char *token = NULL;
    size_t n = 0;
    ssize_t read = 0;
    int num_tokens = 0;
    int i;

    (void)argc;
    printf("Wekcome...\n");

    while (1) {
        printf("%s", prompt);
        read = getline(&lineptr, &n, stdin);
        
        if (read == -1) {
            return (0);
        }
        
        lineptr_copy = malloc(sizeof(char) * read);
        if (lineptr_copy== NULL){
            perror("Quash: memory allocation error");
            return (-1);
        }
        
        strcpy(lineptr_copy, lineptr);

        token = strtok(lineptr, delimiter);
        
        while (token != NULL) {
            num_tokens++;
            token = strtok(NULL, delimiter);
        }
        num_tokens++;

        argv = malloc(sizeof(char *) * num_tokens);

        token = strtok(lineptr_copy, delimiter);

        for (i = 0; token!= NULL; i++) {
            argv[i] = malloc(sizeof(char) * strlen(token));
            strcpy(argv[i], token);
            token = strtok(NULL, delimiter);
        }
        argv[i] = NULL;

        for (i = 0; argv[i] != NULL; i++) {
            printf("%s\n", argv[i]);
        }
        free(lineptr);
        free(lineptr_copy);
        free(argv);
    }

    
    return (0);
}

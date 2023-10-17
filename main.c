#include "main.h"

int main(int argc, char **argv) {
    char *prompt = "[QUASH]$ ";
    char *lineptr;
    size_t n = 0;
    ssize_t read = 0;

    (void)argc;
    (void)argv;
    printf("Wekcome...\n");
    
    while (read != -1) {
        printf("%s", prompt);
        read = getline(&lineptr, &n, stdin);
        printf("%s\n", lineptr);
    }

    
    free(lineptr);
    return (0);
}

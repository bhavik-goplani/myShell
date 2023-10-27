#include "main.h"

void remove_comments(char *input) {
    char *comment_start = strchr(input, '#'); // Find the first occurrence of '#'
    if (comment_start) {
        *comment_start = '\0'; // Truncate the string at the position of '#'
    }
}


char *helper_env_path(char *token) {
    char *path = NULL;
    char *value = NULL;
    
    char *token_copy = strdup(token);
    if (token_copy == NULL) {
        return NULL;
    }

    for (int i = 0; i < (int)strlen(token); i++) {
        if (token[i] == '/') {
            path = token + i;
            break;
        }
    }
    
    char *environ = strtok(token_copy, "/");
    value = getenv(environ);
    
    if (value != NULL && path != NULL) {
        char *result = (char *)malloc(strlen(value) + strlen(path) + 1);
        if (result == NULL) {
            free(token_copy);
            return NULL;
        }
        strcpy(result, value);
        strcat(result, path);
        strcat(result, "\0");
        free(token_copy);  
        return result;
    } else if (value != NULL) {
        char *result = (char *)malloc(strlen(value) + 1);
        if (result == NULL) {
            free(token_copy);
            return NULL;
        }
        strcpy(result, value);
        strcat(result, "\0");
        free(token_copy);  
        return result;
    } else if (path != NULL) {
        char *result = (char *)malloc(strlen(path) + 1);
        if (result == NULL) {

            free(token_copy);
            return NULL;
        }
        strcpy(result, path);
        strcat(result, "\0");
        free(token_copy);  
        return result;
    } else {
        free(token_copy); 
        return NULL;
    }
}


char **helper_remove_quotes(char **argv) {
    char start_quote = '\0';
    for (int i = 0; argv[i] != NULL; i++) {
        char *token = argv[i];
        int token_length = strlen(token);
        int read_index = 0;
        int write_index = 0;

        // Iterate through the string character by character
        while (read_index < token_length) {
            char current_char = token[read_index];

            if (current_char == '\'' || current_char == '\"') {
                // If we encounter a quote character
                if (start_quote == '\0') {
                    // If start_quote is not set, set it
                    start_quote = current_char;
                } else if (start_quote == current_char) {
                    // If start_quote is the same as the current character, it's a closing quote
                    start_quote = '\0';
                } else {
                    // If start_quote is different from the current character, keep it
                    token[write_index] = current_char;
                    write_index++;
                }
            } else {
                // For non-quote characters, simply copy them
                token[write_index] = current_char;
                write_index++;
            }

            read_index++;
        }

        // Null-terminate the modified string
        token[write_index] = '\0';
    }

    return argv;
}

char* join_strings(char **argv, const char *delimiter) {
    int length = 0;
    int i;
    for (i = 0; argv[i] != NULL; i++) {
        length += strlen(argv[i]) + strlen(delimiter);
    }

    char *result = malloc(length + 1);
    result[0] = '\0';

    for (i = 0; argv[i] != NULL; i++) {
        strcat(result, argv[i]);
        if (argv[i + 1] != NULL) {
            strcat(result, delimiter);
        }
    }

    return result;
}
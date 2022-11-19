#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "../include/utils.h"

char *get_file(char *path, const char *file) {
    char *result = malloc(strlen(path) + strlen(file) + 1);
    strcpy(result, path);
    strcat(result, file);
    return result;
}

void debug_parser(char **arr, int n) {
    int i;
    printf("---> ");

    for(i = 0; i < n - 1; i++)
        printf("%s, ", arr[i]);

    printf("%s", arr[i]);
}

int str_to_int(char *str) {
    char *end = NULL;
    errno = 0;
    int result;
    long value = strtol(str, &end, 10);

    if (errno == 0 && *end == '\0') // Boundary check not needed since we are going to do an input validation
        result = (int)value;

    return result;
}

float str_to_float(char *str) {
    char *end = NULL;
    errno = 0;
    float result;
    float value = strtof(str, &end);

    if (errno == 0 && *end == '\0') {
        result = value;
    }
    
    return result;
}

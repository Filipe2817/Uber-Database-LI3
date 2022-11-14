#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "utils.h"

int count_elems(const char *line) {
    int counter = 0;

    for (int i = 0; line[i] != '\0'; i++)
        if (line[i] == ';')
            counter++;

    return (counter + 1);
}

void parse_line(const char *line, int n_elems) { // d = 9, u = 7, r = 10
    int index = 0;
    char **result = malloc(sizeof(char*) * n_elems);    
    char *line_copy, *save;
    char *token = NULL;

    line_copy = save = strdup(line);
    assert(line_copy != NULL);

    while((token = strsep(&line_copy, ";")) != NULL)
        result[index++] = token;

    debug_parser(result, n_elems);
        
    free(save);
    free(result);
}

void parse_file(char *filename) {
    FILE *fptr = NULL;
    fptr = fopen(filename, "r");

    if(fptr == NULL) {
        perror("Error!");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t nread; /* number of characters read, including the delimiter character */
    
    getline(&line, &len, fptr); // 1st line is disposable
    int n = count_elems(line);

    while((nread = getline(&line, &len, fptr)) != -1) {
        parse_line(line, n);
    }
    free(line);
    fclose(fptr);
}

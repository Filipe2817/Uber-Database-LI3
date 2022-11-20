#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../includes/utils.h"
#include "../includes/catalog.h"
#include "../includes/q1.h"

void parse_line(const char *line, char **fields)
{
    int index = 0;
    char *line_copy = NULL;
    char *token = NULL;

    line_copy = strdup(line); // save variable is fields[0]
    assert(line_copy != NULL);

    while ((token = strsep(&line_copy, ";")) != NULL) // don't need to compare n_fields with index (input validation)
        fields[index++] = token;
}

void parse_file(char *filename, int n_fields, void (*function_ptr)(char **, Catalog), Catalog catalog)
{
    FILE *fptr = NULL;
    fptr = fopen(filename, "r");

    if (fptr == NULL)
    {
        perror("Error!");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t nread; /* number of characters read, including the delimiter character */

    (void)getline(&line, &len, fptr); // 1st line is disposable, using void cast to omit compiler warning

    while ((nread = getline(&line, &len, fptr)) != -1)
    {
        char **fields = malloc(sizeof(char *) * n_fields);
        parse_line(line, fields);
        // debug_parser(fields, n_fields);
        (*function_ptr)(fields, catalog);
        free(fields[0]);
        free(fields);
    }
    free(line);
    fclose(fptr);
}

void free_strings(char **ids_usernames, char **remains, int lim)
{
    free_str_array(ids_usernames, lim);
    free_str_array(remains, lim);
}

void parser_q(Catalog catalog)
{
    FILE *inputtxt = fopen("input.txt", "r"); //ficheiro não existe, isto vai ser alterado
    char *input = malloc(500 * sizeof(char));

    while (fgets(input, 500, inputtxt) != NULL)
    {
        input[strcspn(input,"\n")] = '\0';
        if (input[0] == '1')
        {
            input += 2;
            print_q1(input, catalog);
        }
        else if (input[0] == '2')
        {   
            input += 2;
            print_q2(input, catalog);
        }
        else if (input[0] == '3')
        {
            input += 2;
        }
    }
    fclose(inputtxt);
    
}
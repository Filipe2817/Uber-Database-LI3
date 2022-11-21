#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../includes/queries.h"
#include "../includes/utils.h"
#include "../includes/catalog.h"

void handle_query(char **fields, Catalog catalog)
{
    int query_number = str_to_int(fields[0]);

    char *args = fields[1];
    args[strcspn(args, "\n")] = '\0';

    if (query_number == 1)
        print_q1(args, catalog);
    else if (query_number == 2)
        print_q2(args, catalog);
    else if (query_number == 3)
        print_q3(args, catalog);
    else
        return;
}

void print_q1(char *input, Catalog catalog)
{
    char *output = NULL;
    if (isdigit(input[0]))
    {
        output = get_driver_q1(input, catalog);
    }
    else
    {
        output = get_user_q1(input, catalog);
    }
    FILE *results = fopen("Resultados/resultados.txt", "ab");
    if (output == NULL)
    {
        fprintf(results, "\n");
    }
    else
        fprintf(results, "%s\n", output);

    fclose(results);
    free(output);
}

void print_q2(char *input, Catalog catalog)
{
    char *output = NULL;
    int n = str_to_int(input);
    int i = 0, index = 0;

    sort_q2(catalog);

    FILE *results = fopen("Resultados/resultados.txt", "ab");

    while (i < n)
    {
        output = get_q2(index, catalog);

        if (output)
        {
            fprintf(results, "%s\n", output);
            i++;
            free(output);
        }

        index++;
    }

    fclose(results);
}

void print_q3(char *input, Catalog catalog)
{
    char *output = NULL;
    int n = str_to_int(input);
    int i = 0, index = 0;

    sort_q3(catalog);

    FILE *results = fopen("Resultados/resultados.txt", "ab");

    while (i < n)
    {
        output = get_q3(index, catalog);

        if (output)
        {
            fprintf(results, "%s\n", output);
            i++;
            free(output);
        }

        index++;
    }

    fclose(results);
}

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <ctype.h>
#include "../include/q1.h"
#include "../include/utils.h"
#include "../include/catalog.h"

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
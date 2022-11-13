#include <stdio.h> 
#include <stdlib.h>
#include <assert.h>
#include <string.h>  
#include "utils.h"

char **split_string(char *str, int n_elems)
{
    char **result = malloc(sizeof(char *) * n_elems);
    char *token = NULL;
    int index = 0;

    while ((token = strsep(&str, ";")) != NULL)
        result[index++] = token;

    return result;
}

char **parse_line(char *line, int n_elems)
{
    char **result;
    char *line_copy;
    line_copy = strdup(line);
    assert(line_copy != NULL);

    result = split_string(line_copy, n_elems);

    // printf("--> ");
    // for(int i = 0; i < 7; i++)
    //     printf("%s ", result[i]);

    // free(save);
    //  free(result);
    return result;
}

void free_str_array(char **arr, int n)
{
    int i = 0;
    for (i = 0; i < n; i++)
    {
        free(arr[i]);
    }
    free(arr);
}

char *strcat_arr(char **strArr, int lowIndex, int highIndex)
{
    int i = lowIndex;
    char *result = malloc(10000 * sizeof(char));
    result[0] = '\0';
    for (i = lowIndex; i <= highIndex; i++)
    {
        strcat(result, strArr[i]);
        strcat(result, ";");
    }
    return result;
}
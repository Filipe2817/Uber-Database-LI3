#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "../include/user.h"
#include "../include/driver.h"
#include "../include/ride.h"
#include "../include/q1.h"
#include "../include/q3.h"
#include "../include/utils.h"
#include "../include/parser.h"

void free_strings(char **ids_usernames, char **remains, int lim)
{
    free_str_array(ids_usernames, lim);
    free_str_array(remains, lim);
}

void parser()
{
    char **ids_usernames = malloc(100000 * sizeof(char *));
    char **remains = malloc(100000 * sizeof(char *));
    char **outputs_q3 = malloc(100000 * sizeof(char *));
    
    GHashTable *hashD = g_hash_table_new(g_str_hash, g_str_equal);
    GHashTable *hashU = g_hash_table_new(g_str_hash, g_str_equal);
    
    int lim_q1 = build_tables_q1(hashD, hashU, remains, ids_usernames);
    int lim_q3 = get_array_q3(outputs_q3);
    
    FILE *inputtxt = fopen("input.txt", "r");
    char *input = malloc(50 * sizeof(char));
    
    while (fgets(input, 50, inputtxt) != NULL)
    {
        if (input[0] == '1')
        {
            input += 2;
            print_q1(hashD, hashU, input);
        }
        else if (input[0] == '3')
        {
            input += 2;
            print_q3(input, outputs_q3);
        }
    }
    
    fclose(inputtxt);
    free_tables_q1(hashD, hashU);
    free_strings(ids_usernames, remains, lim_q1);
    free_str_array(outputs_q3, lim_q3);
}
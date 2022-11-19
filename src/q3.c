#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <ctype.h>
#include "../include/q1.h"
#include "../include/utils.h"
#include "../include/driver.h"
#include "../include/ride.h"
#include "../include/user.h"
#include "../include/date.h"

void swap(short *a, short *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

// void swap_str(char *str1, char *str2)
// {
//     char *temp = strdup(str1);
//     str2 = str1;
//     str1 = temp;
// }

// function to find the partition position
int partition(short *distance, char **outputs, int *date, char **usernames, char *temp_str, int low, int high)
{

    // select the rightmost element as pivot
    int pivot = distance[high];

    // pointer for greater element
    int i = (low - 1);

    // traverse each element of the array
    // compare them with the pivot
    for (int j = low; j < high; j++)
    {
        if (distance[j] >= pivot)
        {

            // if element smaller than pivot is found
            // swap it with the greater element pointed by i
            i++;

            // swap element at i with element at j
            swap(&distance[i], &distance[j]);
            temp_str = outputs[i];
            outputs[i] = outputs[j];
            outputs[j] = temp_str;
        }
        else if (distance[j] == pivot)
        {
            if (date[j] < date[high])
            {
                swap(&distance[i], &distance[j]);
                temp_str = outputs[i];
                outputs[i] = outputs[j];
                outputs[j] = temp_str;
            }
            else if (strcmp(usernames[j], usernames[high]) < 0)
            {
                swap(&distance[i], &distance[j]);
                temp_str = outputs[i];
                outputs[i] = outputs[j];
                outputs[j] = temp_str;
            }
        }

        // swap the pivot element with the greater element at i
        swap(&distance[i + 1], &distance[high]);
        temp_str = outputs[i + 1];
        outputs[i + 1] = outputs[high];
        outputs[high] = temp_str;

        // return the partition point
        return (i + 1);
    }
}
void quickSort(short *distance, char **outputs, int *date, char **usernames, char *temp_str, int low, int high)
{
    if (low < high)
    {

        // find the pivot element such that
        // elements smaller than pivot are on left of pivot
        // elements greater than pivot are on right of pivot
        int pi = partition(distance, outputs, date, usernames, temp_str, low, high);

        // recursive call on the left of pivot
        quickSort(distance, outputs, date, usernames, temp_str, low, pi - 1);

        // recursive call on the right of pivot
        quickSort(distance, outputs, date, usernames, temp_str, pi + 1, high);
    }
}

// void bubble_sort(short *distance, char **outputs, short *date, char **usernames, int len)
// {
//     int i = 0, j = 0;
//     short temp_dis = 0;
//     char *temp_str = malloc(100 * sizeof(char));

//     for (i = 0; i < len - 1; i++)
//     {
//         for (j = 0; j < len - i - 1; j++)
//         {
//             if (distance[j] < distance[j + 1])
//             {
//                 temp_dis = distance[j];
//                 temp_str = outputs[j];

//                 distance[j] = distance[j + 1];
//                 outputs[j] = outputs[j + 1];

//                 distance[j + 1] = temp_dis;
//                 outputs[j + 1] = temp_str;
//             }
//             else if (distance[j] == distance[j + 1])
//             {
//                 if (date[j] > date[j + 1])
//                 {
//                     temp_dis = distance[j];
//                     temp_str = outputs[j];

//                     distance[j] = distance[j + 1];
//                     outputs[j] = outputs[j + 1];

//                     distance[j + 1] = temp_dis;
//                     outputs[j + 1] = temp_str;
//                 }
//                 else if (strcmp(usernames[j], usernames[j + 1]) > 0)
//                 {
//                     temp_dis = distance[j];
//                     temp_str = outputs[j];

//                     distance[j] = distance[j + 1];
//                     outputs[j] = outputs[j + 1];

//                     distance[j + 1] = temp_dis;
//                     outputs[j + 1] = temp_str;
//                 }
//             }
//         }
//     }
// }

short *get_format_rides_data_q3(GHashTable *usernames_to_index, char **outputs, short *account_status, int *date, int lim)
{
    int i = 0, index = 0;
    int date_value = 0;
    short *distance = calloc(100000, sizeof(short));
    FILE *rides = fopen("dataF1/rides.csv", "r");
    char *data = malloc(300 * sizeof(char));
    char **lineArr;

    fgets(data, 300, rides);
    while (fgets(data, 300, rides) != NULL)
    {
        lineArr = parse_line(data, 10);
        if (account_status[i] == 0)
        {
            date[index] = 65535; // deve ser maior que qualquer data;
            index = (int)g_hash_table_lookup(usernames_to_index, lineArr[3]);
            distance[index] += atoi(lineArr[5]);
            date_value = date_to_int(lineArr[1]);
            if (date[index] > date_value)
                date[index] = date_value;
        }
    }

    fclose(rides);
    free(data);
    free(lineArr);

    char *distance_str = malloc(10 * sizeof(char));

    for (i = 0; i < lim; i++)
    {
        distance_str[0] = '\0';
        sprintf(distance_str, "%hu", distance[i]);
        strcat(outputs[i], distance_str);
    }

    return distance;
}

int parser_user_q3(char **outputs)
{
    int i = 0;

    FILE *users = fopen("dataF1/users.csv", "r");
    char **usernames = malloc(100000 * sizeof(char *));
    GHashTable *usernames_to_index = g_hash_table_new(g_str_hash, g_str_equal);
    short *account_status = calloc(100000, sizeof(short));
    char *data = malloc(300 * sizeof(char));
    char **lineArr;

    fgets(data, 300, users);
    while (fgets(data, 300, users) != NULL)
    {

        lineArr = parse_line(data, 7);
        if (strcmp(lineArr[6], "inactive\n") == 0)
        {
            account_status[i] = 1;
        }
        else
        {
            usernames[i] = lineArr[0];
            g_hash_table_insert(usernames_to_index, lineArr[0], GINT_TO_POINTER(i));
            outputs[i] = strcat_user_data_q3(lineArr);
            i++;
        }
    }

    fclose(users);
    free(data);
    free(lineArr);

    int *date = malloc(1000000 * sizeof(int));

    short *distance = get_format_rides_data_q3(usernames_to_index, outputs, account_status, date, i);

    g_hash_table_destroy(usernames_to_index);

    // bubble_sort(distance, outputs, date, usernames, i);
    char *temp_str = malloc(100 * sizeof(char));
    quickSort(distance, outputs, date, usernames, temp_str, 0, i - 1);

    free(temp_str);
    free(distance);
    free(account_status);
    free(usernames);

    return i;
}

int get_array_q3(char **outputs)
{
    int lim = parser_user_q3(outputs);
    return lim;
}

void print_q3(char *input, char **outputs)
{
    int n_out = 0, i = 0;
    input[strcspn(input, "\n")] = '\0';
    if (isdigit(input[0]))
    {
        n_out = atoi(input);
        FILE *results = fopen("Resultados/resultados.txt", "ab");
        for (i = 0; i < n_out; i++)
        {
            fprintf(results, "%s\n", outputs[i]);
        }
        fclose(results);
    }
}
// int scan_print_q3(char **outputs)
// {
//     int i = 0;
//     int lim = parser_user_q3(outputs);
//     int n_output = 0;

//     if (isdigit(input[0]))
//     {
//         n_output = atoi(input);
//         FILE *results = fopen("Resultados/resultados.txt", "ab");
//         for (i = 0; i <= n_output; i++)
//         {
//             fprintf(results, "%s\n", outputs[i]);
//         }
//         fclose(results);
//     }
// }

// void q3()
// {
//     char **outputs = malloc(100000 * sizeof(char *));
//     int lim = scan_print_q3(outputs);
//     free_str_array(outputs, lim);
// }
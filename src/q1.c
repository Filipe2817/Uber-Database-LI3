#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include "q1.h"
#include "utils.h"
#include "driver.h"
#include "ride.h"
#include "user.h"

void get_format_rides_data(float *rating, int *ratings_quantity, int *total_rides, float *tips, char **remains, int size)
{
    int i = 0;
    FILE *rides = fopen("dataF1/rides.csv", "r");
    char *data = malloc(100000 * sizeof(char));
    char *rating_str = malloc(100 * sizeof(char));
    char *total_rides_str = malloc(5 * sizeof(char));
    char *tips_str = malloc(100 * sizeof(char));
    char **lineArr;
    fgets(data, 100000, rides);
    while (fgets(data, 100000, rides) != NULL)
    {
        lineArr = parse_line(data, 10);
        rating[atoi(lineArr[2]) - 1] += atof(lineArr[7]);
        ratings_quantity[atoi(lineArr[2]) - 1] += 1;
        total_rides[atoi(lineArr[2]) - 1] += 1;
        tips[atoi(lineArr[2]) - 1] += atof(lineArr[8]);
    }
    free(lineArr);
    free(data);
    fclose(rides);
    for (i = 0; i < size; i++)
    {
        rating_str[0] = '\0';
        total_rides_str[0] = '\0';
        tips_str[0] = '\0';
        rating[i] = rating[i] / ratings_quantity[i];
        gcvt(rating[i], 4, rating_str);
        sprintf(total_rides_str, "%d", total_rides[i]);
        gcvt(tips[i], 4, tips_str);
        strcat(remains[i], rating_str);
        strcat(remains[i], ";");
        strcat(remains[i], total_rides_str);
        strcat(remains[i], ";");
        strcat(remains[i], tips_str);
    }
    free(rating_str);
    free(total_rides_str);
    free(tips_str);
}

int parser_driver(GHashTable *hashD, char **ids, char **remains)
{
    int i = 0, u = 0;
    FILE *drivers = fopen("dataF1/drivers.csv", "r");
    if (drivers == NULL)
    {
        fprintf(stderr, "sem ficheiro");
    }
    char *data = malloc(10000);
    char **lineArr;
    fgets(data, 10000, drivers);
    while (fgets(data, 10000, drivers) != NULL)
    {
        lineArr = parse_line(data, 9);
        remains[i] = strcat_arr(lineArr, 1, 3);
        ids[i] = lineArr[0];
        i++;
    }
    free(lineArr);
    free(data);
    fclose(drivers);
    float *rating = calloc(100000, sizeof(float));
    int *ratings_quantity = calloc(100000, sizeof(int));
    int *total_rides = calloc(100000, sizeof(int));
    float *tips = calloc(100000, sizeof(float));
    get_format_rides_data(rating, ratings_quantity, total_rides, tips, remains, 10000);
    for (u = 0; u < i; u++)
    {
        g_hash_table_insert(hashD, ids[u], remains[u]);
    }
    free(rating);
    free(ratings_quantity);
    free(total_rides);
    free(tips);
    return i;
}

void parser_q1()
{
    int lim = 0;
    GHashTable *hashTable = g_hash_table_new(g_str_hash, g_str_equal);
    char **remains = malloc(100000 * sizeof(char *));
    char **ids = malloc(10000 * sizeof(char *));
    lim = parser_driver(hashTable, ids, remains);
    printf("id: %s\ndata: %s\n", "100", (char *)g_hash_table_lookup(hashTable, "000000000100"));
    g_hash_table_destroy(hashTable);
    free_str_array(remains, lim);
    free_str_array(ids, lim);
}
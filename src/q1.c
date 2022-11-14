#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include "q1.h"
#include "utils.h"
#include "driver.h"
#include "ride.h"
#include "user.h"

float get_starting_money(char *car_class)
{
    float money = 0;
    if (strcmp(car_class, "basic") == 0)
    {
        money = 3.25;
    }
    else if (strcmp(car_class, "green") == 0)
    {
        money = 4.00;
    }
    else if (strcmp(car_class, "premium") == 0)
    {
        money = 5.20;
    }
    return money;
}

float get_money_km(char *car_class)
{
    float money_km = 0;
    if (strcmp(car_class, "basic") == 0)
    {
        money_km = 0.62;
    }
    else if (strcmp(car_class, "green") == 0)
    {
        money_km = 0.79;
    }
    else if (strcmp(car_class, "premium") == 0)
    {
        money_km = 0.94;
    }
    return money_km;
}

void get_format_rides_data(float *rating, int *ratings_quantity, int *total_rides, float *money, char **car_classes, char **remains, int size)
{
    int i = 0, index_money = 0;

    FILE *rides = fopen("dataF1/rides.csv", "r");
    char *data = malloc(100000 * sizeof(char));
    char *rating_str = malloc(100 * sizeof(char));
    char *total_rides_str = malloc(5 * sizeof(char));
    char *money_str = malloc(100 * sizeof(char));
    char **lineArr;

    fgets(data, 100000, rides);
    while (fgets(data, 100000, rides) != NULL)
    {
        lineArr = parse_line(data, 10);
        rating[atoi(lineArr[2]) - 1] += atof(lineArr[7]);
        ratings_quantity[atoi(lineArr[2]) - 1] += 1;
        total_rides[atoi(lineArr[2]) - 1] += 1;
        money[atoi(lineArr[2]) - 1] += (atof(lineArr[8]) + atof(lineArr[5]) * get_money_km(car_classes[atoi(lineArr[2]) - 1]));
    }

    for (index_money = 0; index_money < size; index_money++)
    {
        money[index_money] += get_starting_money(car_classes[index_money]) * total_rides[index_money];
    }

    free(lineArr);
    free(data);
    fclose(rides);

    for (i = 0; i < size; i++)
    {
        rating_str[0] = '\0';
        total_rides_str[0] = '\0';
        money_str[0] = '\0';
        rating[i] = rating[i] / ratings_quantity[i];
        sprintf(rating_str, "%.3f", rating[i]);
        sprintf(total_rides_str, "%d", total_rides[i]);
        sprintf(money_str, "%.3f", money[i]);
        strcat(remains[i], rating_str);
        strcat(remains[i], ";");
        strcat(remains[i], total_rides_str);
        strcat(remains[i], ";");
        strcat(remains[i], money_str);
    }

    free(rating_str);
    free(total_rides_str);
    free(money_str);
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
    char **car_classes = malloc(10000 * sizeof(char *));

    fgets(data, 10000, drivers);
    while (fgets(data, 10000, drivers) != NULL)
    {
        lineArr = parse_line(data, 9);
        remains[i] = strcat_arr(lineArr, 1, 3);
        ids[i] = lineArr[0];
        car_classes[i] = lineArr[4];
        i++;
    }

    free(lineArr);
    free(data);
    fclose(drivers);

    float *rating = calloc(100000, sizeof(float));
    int *ratings_quantity = calloc(100000, sizeof(int));
    int *total_rides = calloc(100000, sizeof(int));
    float *money = calloc(100000, sizeof(float));

    get_format_rides_data(rating, ratings_quantity, total_rides, money, car_classes, remains, 10000);

    for (u = 0; u < i; u++)
    {
        g_hash_table_insert(hashD, ids[u], remains[u]);
    }

    free(car_classes);
    free(rating);
    free(ratings_quantity);
    free(total_rides);
    free(money);

    return i;
}

int spcoise(GHashTable *HashTable, char **ids, char **remains)
{
    char *input = malloc(15 * sizeof(char));
    input[0] = '\0';
    int lim = 0;
    fgets(input, 15, stdin);
    if (input[0] == '1' && input[1] == ' ')
    {
        input += 2;
        lim = parser_driver(HashTable, ids, remains);
        printf("%s\n", (char *)g_hash_table_lookup(HashTable, input));
    }
    return lim;
}

void parser_q1()
{
    int lim = 0;
    GHashTable *hashTable = g_hash_table_new(g_str_hash, g_str_equal);
    char **remains = malloc(100000 * sizeof(char *));
    char **ids = malloc(10000 * sizeof(char *));
    // lim = parser_driver(hashTable, ids, remains);
    // printf("id: %s\ndata: %s\n", "100", (char *)g_hash_table_lookup(hashTable, "000000000100"));
    lim = spcoise(hashTable, ids, remains);
    g_hash_table_destroy(hashTable);
    free_str_array(remains, lim);
    free_str_array(ids, lim);
}
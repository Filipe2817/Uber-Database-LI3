#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <ctype.h>
#include "q1.h"
#include "utils.h"
#include "driver.h"
#include "ride.h"
#include "user.h"
#include "date.h"

// idades contam-se a 9/10/22

void print_hasht(GHashTable *t, char **keys, int l)
{
    int i = 0;
    for (i = 0; i < l; i++)
    {
        printf("%s\n", (char *)g_hash_table_lookup(t, keys[i]));
    }
}

int username_index_link(char **usernames, char *user, int size)
{
    int i = 0;
    for (i = 0; i < size; i++)
    {
        if (strcmp(user, usernames[i]) == 0)
            return i;
    }
    return -1;
}

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

void get_format_rides_data_driver(float *rating, int *ratings_quantity, int *total_rides, float *money, char **car_classes, char **remains, int size)
{
    int i = 0;

    FILE *rides = fopen("dataF1/rides.csv", "r");
    char *data = malloc(300 * sizeof(char));
    char *rating_str = malloc(10 * sizeof(char));
    char *total_rides_str = malloc(10 * sizeof(char));
    char *money_str = malloc(10 * sizeof(char));
    char **lineArr;

    fgets(data, 300, rides);
    while (fgets(data, 300, rides) != NULL)
    {
        lineArr = parse_line(data, 10);
        rating[atoi(lineArr[2]) - 1] += atof(lineArr[7]);
        ratings_quantity[atoi(lineArr[2]) - 1] += 1;
        total_rides[atoi(lineArr[2]) - 1] += 1;
        money[atoi(lineArr[2]) - 1] += (atof(lineArr[8]) + atof(lineArr[5]) * get_money_km(car_classes[atoi(lineArr[2]) - 1]) + get_starting_money(car_classes[atoi(lineArr[2]) - 1]));
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

    char *data = malloc(300 * sizeof(char));
    char **lineArr;
    char **car_classes = malloc(10000 * sizeof(char *));
    short *account_status = calloc(10000, sizeof(short));

    fgets(data, 300, drivers);
    while (fgets(data, 300, drivers) != NULL)
    {
        lineArr = parse_line(data, 9);
        ids[i] = lineArr[0];
        remains[i] = strcat_driver_data(lineArr);
        car_classes[i] = lineArr[4];
        if (strcmp(lineArr[8], "inactive\n") == 0)
        {
            account_status[i] = 1;
        }
        i++;
    }

    // active = 0 e inactive = 1, executa menos vezes a instrução assim

    free(lineArr);
    free(data);
    fclose(drivers);

    float *rating = calloc(100000, sizeof(float));
    int *ratings_quantity = calloc(100000, sizeof(int));
    int *total_rides = calloc(100000, sizeof(int));
    float *money = calloc(100000, sizeof(float));

    get_format_rides_data_driver(rating, ratings_quantity, total_rides, money, car_classes, remains, i);

    for (u = 0; u < i; u++)
    {
        if (account_status[u] == 0)
        {
            g_hash_table_insert(hashD, ids[u], remains[u]);
        }
        else
        {
            g_hash_table_insert(hashD, ids[u], "\0");
        }
    }

    free(account_status);
    free(car_classes);
    free(rating);
    free(ratings_quantity);
    free(total_rides);
    free(money);

    return i;
}

void get_format_rides_data_user(float *rating, short *ratings_quantity, short *total_rides, float *money, GHashTable *car_classes, GHashTable *usernames_to_index, char **remains, int size)
{
    int i = 0, index = 0;

    FILE *rides = fopen("dataF1/rides.csv", "r");
    char *data = malloc(300 * sizeof(char));
    char *rating_str = malloc(10 * sizeof(char));
    char *total_rides_str = malloc(10 * sizeof(char));
    char *money_str = malloc(10 * sizeof(char));
    char **lineArr;

    fgets(data, 300, rides);
    while (fgets(data, 300, rides) != NULL)
    {
        lineArr = parse_line(data, 10);
        index = (int)g_hash_table_lookup(usernames_to_index, lineArr[3]);
        rating[index] += atof(lineArr[6]);
        ratings_quantity[index] += 1;
        total_rides[index] += 1;
        money[index] += (atof(lineArr[8]) + atof(lineArr[5]) * get_money_km((char *)g_hash_table_lookup(car_classes, lineArr[2])) + get_starting_money((char *)g_hash_table_lookup(car_classes, lineArr[2])));
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

int parser_user(GHashTable *hashD, char **usernames, char **remains)
{
    int i = 0, u = 0;
    GHashTable *car_classes = g_hash_table_new(g_str_hash, g_str_equal);

    FILE *drivers = fopen("dataF1/drivers.csv", "r");

    char *data = malloc(300 * sizeof(char));
    char **lineArr;

    fgets(data, 300, drivers);
    while (fgets(data, 300, drivers) != NULL)
    {
        lineArr = parse_line(data, 9);
        g_hash_table_insert(car_classes, lineArr[0], lineArr[4]);
    }

    fclose(drivers);
    FILE *users = fopen("dataF1/users.csv", "r");

    GHashTable *username_to_index = g_hash_table_new(g_str_hash, g_str_equal);
    short *account_status = calloc(100000, sizeof(short));

    fgets(data, 300, users);
    while (fgets(data, 300, users) != NULL)
    {
        lineArr = parse_line(data, 7);
        usernames[i] = lineArr[0];
        g_hash_table_insert(username_to_index, lineArr[0], GINT_TO_POINTER(i));
        remains[i] = strcat_user_data(lineArr);
        if (strcmp(lineArr[6], "inactive\n") == 0)
        {
            account_status[i] = 1;
        }
        i++;
    }

    // active = 0 e inactive = 1, executa menos vezes a instrução assim

    free(lineArr);
    free(data);
    fclose(users);

    float *rating = calloc(100000, sizeof(float));
    short *ratings_quantity = calloc(100000, sizeof(short));
    short *total_rides = calloc(100000, sizeof(short));
    float *money = calloc(100000, sizeof(float));

    get_format_rides_data_user(rating, ratings_quantity, total_rides, money, car_classes, username_to_index, remains, i);

    g_hash_table_destroy(car_classes);
    g_hash_table_destroy(username_to_index);

    for (u = 0; u < i; u++)
    {
        if (account_status[u] == 0)
        {
            g_hash_table_insert(hashD, usernames[u], remains[u]);
        }
        else
        {
            g_hash_table_insert(hashD, usernames[u], "\0");
        }
    }

    // fprintf(stdout,"%s\n", (char *)g_hash_table_lookup(hashD, "MiTeixeira")); -> aqui funciona

    free(account_status);
    free(rating);
    free(ratings_quantity);
    free(total_rides);
    free(money);

    return i;
}

int scan_print(GHashTable *HashD, GHashTable *HashU, char **ids_usernames, char **remains)
{
    char *input = malloc(50 * sizeof(char));
    input[0] = '\0';
    int lim = 0;

    int pd = parser_driver(HashD, ids_usernames, remains);
    int pu = parser_user(HashU, ids_usernames, remains);

    if (pu > pd)
        lim = pu;
    else
        lim = pd;

    FILE *inputxt = fopen("inputQ1.txt", "r");
    while (fgets(input, 50, inputxt) != NULL)
    {
        if (input[0] == '1' && input[1] == ' ')
        {
            input += 2;
            input[strcspn(input, "\n")] = '\0';
            if (isdigit(input[0]))
            {
                FILE *results = fopen("Resultados/resultados.txt", "ab");
                fprintf(results, "%s\n", (char *)g_hash_table_lookup(HashD, input));
                fclose(results);
            }
            else
            {

                FILE *results = fopen("Resultados/resultados.txt", "ab");
                fprintf(results, "%s\n", (char *)g_hash_table_lookup(HashU, input));
                fclose(results);
            }
        }
    }
    fclose(inputxt);
    return lim;
}

void q1()
{
    int lim = 0;
    GHashTable *hashD = g_hash_table_new(g_str_hash, g_str_equal);
    GHashTable *hashU = g_hash_table_new(g_str_hash, g_str_equal);
    char **remains = malloc(100000 * sizeof(char *));
    char **ids_usernames = malloc(100000 * sizeof(char *));
    lim = scan_print(hashD, hashU, ids_usernames, remains);
    g_hash_table_destroy(hashD);
    g_hash_table_destroy(hashU);
    free_str_array(remains, lim);
    free_str_array(ids_usernames, lim);
}
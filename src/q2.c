#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <ctype.h>
#include "../includes/q2.h"
#include "../includes/utils.h"
#include "../includes/catalog.h"

int compare_driver_id (Driver d1, Diver d2)
{
    int r = 0;

    int id1 = str_to_int (d1->id);
    int id2 = str_to_int (d1->id);

    if (id1 <= id2) r = 1;

    return r;
}

int compare_ride_dates (Ride r1, Ride r2)
{
    int r = 0;

    if (r1->date <= r2->date) r = 1;

    return r;
}

Ride get_driver_most_recent_ride (Driver d1)
{
    int i = 0;
    Ride ride, most_recent_ride;
    most_recent_ride_date = 0;

    for (int i=0; i<catalog->rides_array->len; i++)
    {
        ride = g_ptr_array_index(catalog->rides_array, i);
        char *driver_id = get_ride_driver_id(ride);

        if (strcmp(driver->id, driver_id) == 0 && (compare_ride_dates (most_recent_ride, ride) == 1)) 
        {
            most_recent_ride = ride;
            most_recent_ride->date = ride->date;
        }

        free(driver_id);
    }

    return most_recent_ride;
}

int compare_average_score (Driver d1, Driver d2)
{
    int r = 0;
    double average_score_d1 = get_average_rating (d1);
    double average_score_d2 = get_average_rating (d2);

    if (average_score_d1 <= average_score_d2) r = 1;

    return r;
}

double get_average_rating (Driver driver)
{
    double average_rating = 0;
    int totla_rides = 0;

    Ride ride;

    for (int i=0; i<catalog->rides_array->len; i++)
    {
        ride = g_ptr_array_index(catalog->rides_array, i);
        char *driver_id = get_ride_driver_id(ride);

        if (strcmp(driver->id, driver_id) == 0) 
        {
            average_rating += get_ride_driver_score(ride);
            total_rides++;
        }

        free(driver_id);

        average_rating = average_rating / total_rides;
    }

    return average_rating;
}

void sort_drivers_arr (Catalog catalog)
{
    int len = catalog->drivers_array->len;  
    int i, j;

    Driver temp;

    for (int i=0; i<len; i++)
    {
        for (int j=0; j<len; j++)
        {
            if (compare_average_score (catalog->drivers_array[i], catalog->drivers_array[j]) == 1)
            {
                Ride most_recent_ride1 = get_driver_most_recent_ride (catalog->drivers_array[i]);
                Ride most_recent_ride2 = get_driver_most_recent_ride (catalog->drivers_array[j]);

                if (compare_ride_dates (most_recent_ride1, most_recent_ride2) == 1)
                {
                    if (compare_driver_id (catalog->drivers_array[i], catalog->drivers_array[j]) == 1)
                    {
                        temp = catalog->drivers_array[i];
                        catalog->drivers_array[i] = catalog->drivers_array[j];
                        catalog->drivers_array[j] = temp;
                    }
                }
            }
        }
    }
}

void print_first_n (int n, Catalog catalog)
{
    sort_drivers_arr (catalog);

    char *id;
    char *name;
    double average_score;

    for (int i=0; i<n; i++)
    {
        id = catalog->drivers_array[i]->id;
        name = catalog->drivers_array[i]->name;
        average_score = get_average_rating (catalog->drivers_array[i]);

        printf ("%s;%s;%.3f\n", id, name, average_score);

        free (id);
        free (name);
        free (average_score);
    }
}

void print_q2 (char *input, Catalog catalog)
{
    char *output = NULL;

    if (isdigit(input[0]))
    {
        
    }

    FILE *results = fopen ("Resultados/resultados.txt", "ab");

    if (output == NULL)
    {
        fprint (results, "\n");
    }

    else
    {
        fprintf(results, "%s\n", output);
        fclose(results);
        free(output);
    }
}
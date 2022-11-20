#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include "../includes/catalog.h"
#include "../includes/user.h"
#include "../includes/driver.h"
#include "../includes/ride.h"

// g_ptr_array_index(arr,index)

typedef struct catalog
{
    // GPtrArray *users_array;
    GPtrArray *drivers_array;
    GPtrArray *rides_array;
    GHashTable *users_ht;
    GHashTable *drivers_ht;
} * Catalog;

void glib_wrapper_free_user(gpointer user)
{
    free_user(user);
}

void glib_wrapper_free_driver(gpointer driver)
{
    free_driver(driver);
}

void glib_wrapper_free_ride(gpointer ride)
{
    free_ride(ride);
}

Catalog create_catalog()
{
    Catalog catalog = malloc(sizeof(struct catalog));
    catalog->drivers_array = g_ptr_array_new_with_free_func (glib_wrapper_free_driver);
    catalog->rides_array = g_ptr_array_new_with_free_func(glib_wrapper_free_ride);
    catalog->users_ht = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, glib_wrapper_free_user);
    catalog->drivers_ht = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, glib_wrapper_free_driver);

    return catalog;
}

void insert_user_in_catalog(char **fields, Catalog catalog)
{
    User user = create_user(fields);
    char *key = get_user_username(user);
    g_hash_table_insert(catalog->users_ht, key, user);
}

void insert_driver_in_catalog(char **fields, Catalog catalog)
{
    Driver driver = create_driver(fields);
    char *key = get_driver_id(driver);
    g_hash_table_insert(catalog->drivers_ht, key, driver);
    g_ptr_array_add(catalog->drivers_array, driver);
}

void insert_ride_in_catalog(char **fields, Catalog catalog)
{
    Ride ride = create_ride(fields);
    g_ptr_array_add(catalog->rides_array, ride);
}

double get_ride_cost(char *car_class, unsigned short distance)
{
    double result = 0;

    if (strcmp(car_class, "basic") == 0)
    {
        result = 3.25 + 0.62 * distance;
    }
    else if (strcmp(car_class, "green") == 0)
    {
        result = 4 + 0.79 * distance;
    }
    else
    {
        result = 5.20 + 0.94 * distance;
    }

    return result;
}

char *get_driver_q1(char *id, Catalog catalog)
{
    Driver driver = g_hash_table_lookup(catalog->drivers_ht, id);

    if (!get_driver_account_status(driver))
        return NULL;

    char *name = get_driver_name(driver);
    char *gender = get_driver_gender(driver);
    char *age = get_driver_age(driver);

    Ride ride;
    unsigned int i = 0;
    double average_rating = 0;
    double money = 0;
    unsigned short total_rides = 0;
    unsigned short distance = 0;

    for (i = 0; i < catalog->rides_array->len; i++)
    {
        ride = g_ptr_array_index(catalog->rides_array, i);
        char *driver_id = get_ride_driver_id(ride);

        if (strcmp(id, driver_id) == 0)
        {
            char *car_class = get_driver_car_class(g_hash_table_lookup(catalog->drivers_ht, driver_id));
            distance = get_ride_distance(ride);
            total_rides++;
            average_rating += get_ride_driver_score(ride);
            money += get_ride_cost(car_class, distance) + get_ride_tip(ride);

            free(car_class);
        }
        free(driver_id);
    }

    average_rating = average_rating / total_rides;

    char *driver_str = malloc(strlen(name) + strlen(gender) + strlen(age) + 5 + 10 + 10 + 5 + 1); // 5 de rating, 10 de total_rides, 10 de money, 5 dos ;, 1 do \0

    sprintf(driver_str, "%s;%s;%s;%.3f;%hu;%.3f", name, gender, age, average_rating, total_rides, money);

    free(name);
    free(gender);
    free(age);

    return driver_str;
}

char *get_user_q1(char *username, Catalog catalog)
{
    User user = g_hash_table_lookup(catalog->users_ht, username);

    if (!get_user_account_status(user))
        return NULL;

    char *name = get_user_name(user);
    char *gender = get_user_gender(user);
    char *age = get_user_age(user);

    Ride ride;
    unsigned int i = 0;
    double average_rating = 0;
    double money = 0;
    unsigned short total_rides = 0;
    unsigned short distance = 0;

    for (i = 0; i < catalog->rides_array->len; i++)
    {
        ride = g_ptr_array_index(catalog->rides_array, i);
        char *user_username = get_ride_user_username(ride);

        if (strcmp(username, user_username) == 0)
        {
            char *driver_id = get_ride_driver_id(ride);
            char *car_class = get_driver_car_class(g_hash_table_lookup(catalog->drivers_ht, driver_id));
            distance = get_ride_distance(ride);
            total_rides++;
            average_rating += get_ride_user_score(ride);
            money += get_ride_cost(car_class, distance) + get_ride_tip(ride);

            free(car_class);
            free(driver_id);
        }
        free(user_username);
    }

    average_rating = average_rating / total_rides;

    char *user_str = malloc(strlen(name) + strlen(gender) + strlen(age) + 5 + 10 + 10 + 5 + 1); // 5 de rating, 10 de total_rides, 10 de money, 5 dos ;, 1 do \0

    sprintf(user_str, "%s;%s;%s;%.3f;%hu;%.3f", name, gender, age, average_rating, total_rides, money);

    free(name);
    free(gender);
    free(age);

    return user_str;
}

void free_catalog(Catalog catalog)
{
    g_ptr_array_free(catalog->rides_array, TRUE);
    g_hash_table_destroy(catalog->users_ht);
    g_hash_table_destroy(catalog->drivers_ht);
    free(catalog);
}

// For debug purposes
void glib_wrapper_print_ride(gpointer ride, gpointer user_data)
{
    (void)user_data; // cast to omit warning
    print_ride(ride);
}

void print_catalog(Catalog catalog)
{
    GHashTableIter iter1;
    gpointer key1, value1;

    g_hash_table_iter_init(&iter1, catalog->users_ht);
    while (g_hash_table_iter_next(&iter1, &key1, &value1))
    {
        printf("Key: %s ---> Value:", (char *)key1); // cast just to remove warning (this function will be deleted)
        print_user(value1);
    }

    GHashTableIter iter2;
    gpointer key2, value2;

    g_hash_table_iter_init(&iter2, catalog->drivers_ht);
    while (g_hash_table_iter_next(&iter2, &key2, &value2))
    {
        printf("Key: %s ---> Value:", (char *)key2);
        print_driver(value2);
    }

    g_ptr_array_foreach(catalog->rides_array, glib_wrapper_print_ride, NULL);
}

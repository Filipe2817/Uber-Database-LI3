#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/catalog.h"
#include "../include/user.h"
#include "../include/driver.h"
#include "../include/ride.h"

typedef struct catalog {
    /*
    GPtrArray *users_array;
    GPtrArray *drivers_array;
    */
    GPtrArray *rides_array;
    GHashTable *users_ht;
    GHashTable *drivers_ht;
} *Catalog;

void glib_wrapper_free_user(gpointer user) {
    free_user(user);
}

void glib_wrapper_free_driver(gpointer driver) {
    free_driver(driver);
}

void glib_wrapper_free_ride(gpointer ride) {
    free_ride(ride);
}

Catalog create_catalog() {
    Catalog catalog = malloc(sizeof(struct catalog));

    catalog->rides_array = g_ptr_array_new_with_free_func(glib_wrapper_free_ride);
    catalog->users_ht = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, glib_wrapper_free_user);
    catalog->drivers_ht = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, glib_wrapper_free_driver);

    return catalog;
}

void insert_user_in_catalog(char **fields, Catalog catalog) {
    User user = create_user(fields);
    char *key = get_user_username(user);
    g_hash_table_insert(catalog->users_ht, key, user);
}

void insert_driver_in_catalog(char **fields, Catalog catalog) {
    Driver driver = create_driver(fields);
    char *key = get_driver_id(driver);
    g_hash_table_insert(catalog->drivers_ht, key, driver);
}

void insert_ride_in_catalog(char **fields, Catalog catalog) {
    Ride ride = create_ride(fields);
    g_ptr_array_add(catalog->rides_array, ride);
}

void free_catalog(Catalog catalog) {
    g_ptr_array_free(catalog->rides_array, TRUE);
    g_hash_table_destroy(catalog->users_ht);
    g_hash_table_destroy(catalog->drivers_ht);
    free(catalog);
}

// For debug purposes
void glib_wrapper_print_ride(gpointer ride, gpointer user_data) {
    (void)user_data; // cast to omit warning
    print_ride(ride);
}

void print_catalog(Catalog catalog) {
    GHashTableIter iter1;
    gpointer key1, value1;

    g_hash_table_iter_init (&iter1, catalog->users_ht);
    while (g_hash_table_iter_next (&iter1, &key1, &value1)) {
        printf("Key: %s ---> Value:", (char *)key1); // cast just to remove warning (this function will be deleted)
        print_user(value1);
    }

    GHashTableIter iter2;
    gpointer key2, value2;

    g_hash_table_iter_init (&iter2, catalog->drivers_ht);
    while (g_hash_table_iter_next (&iter2, &key2, &value2)) {
        printf("Key: %s ---> Value:", (char *)key2);
        print_driver(value2);
    }

    g_ptr_array_foreach(catalog->rides_array, glib_wrapper_print_ride, NULL);
}


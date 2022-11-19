#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/parser.h"
#include "../include/utils.h"
#include "../include/catalog.h"

#define N_USER_FIELDS 7
#define N_DRIVER_FIELDS 9
#define N_RIDE_FIELDS 10

int main(int argc, char **argv) {
    if(argc == 2) {
        char *path = argv[1];

        char *users_file = get_file(path, "/users.csv");
        char *drivers_file = get_file(path, "/drivers.csv");        
        char *rides_file = get_file(path, "/rides.csv");

        Catalog catalog = create_catalog();

        parse_file(users_file, N_USER_FIELDS, insert_user_in_catalog, catalog);
        parse_file(drivers_file, N_DRIVER_FIELDS, insert_driver_in_catalog, catalog);
        parse_file(rides_file, N_RIDE_FIELDS, insert_ride_in_catalog, catalog);

        //print_catalog(catalog);

        free_catalog(catalog);
        free(users_file);
        free(drivers_file);
        free(rides_file);
    }
    else {
        printf("Insufficient number of arguments!");
    }

    return 0;
}
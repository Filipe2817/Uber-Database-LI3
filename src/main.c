#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "utils.h"

int main(int argc, char **argv) {
    if(argc == 2) {
        char *path = argv[1];

        char *users_file = get_file(path, "/users.csv");
        char *drivers_file = get_file(path, "/drivers.csv");        
        char *rides_file = get_file(path, "/rides.csv");

        parse_file(users_file);
        parse_file(drivers_file);
        parse_file(rides_file);

        free(users_file);
        free(drivers_file);
        free(rides_file);
    }
    else {
        printf("Insufficient number of arguments!");
    }

    return 0;
}
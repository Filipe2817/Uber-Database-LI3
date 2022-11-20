#ifndef DRIVER_H
#define DRIVER_H

#include <stdbool.h>

typedef struct driver *Driver;

Driver create_driver(char **fields);

char *get_driver_id(Driver driver);

void free_driver(Driver driver);

char *get_driver_name(Driver driver);

char *get_driver_gender(Driver driver);

char *get_driver_age(Driver driver);

char *get_driver_car_class(Driver driver);

bool get_driver_account_status(Driver driver);

void print_driver(Driver driver);

#endif
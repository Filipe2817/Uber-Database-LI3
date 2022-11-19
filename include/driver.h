#ifndef DRIVER_H
#define DRIVER_H

typedef struct driver *Driver;

Driver create_driver(char **fields);

char *get_driver_id(Driver driver);

void free_driver(Driver driver);

void print_driver(Driver driver);

#endif
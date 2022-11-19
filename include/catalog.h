#ifndef CATALOG_H
#define CATALOG_H

typedef struct catalog *Catalog;

Catalog create_catalog();

void free_catalog(Catalog catalog);

void insert_user_in_catalog(char **fields, Catalog catalog);

void insert_driver_in_catalog(char **fields, Catalog catalog);

void insert_ride_in_catalog(char **fields, Catalog catalog);

void print_catalog(Catalog catalog);

#endif
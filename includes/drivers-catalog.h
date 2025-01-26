#ifndef DRIVERS_CATALOG_H
#define DRIVERS_CATALOG_H

#include <stdbool.h>
#include "../includes/vp_array.h"

typedef struct drivers_catalog *Drivers_Catalog;

Drivers_Catalog create_drivers_catalog();

int is_valid_driver(char **fields);

void insert_driver_in_catalog(char **fields, va_list args);

void update_driver_stats(unsigned int driver_id, VPA *stats, Drivers_Catalog catalog);

char *get_ride_car_class(unsigned int driver_id, Drivers_Catalog catalog);

char *get_driver_name_id(unsigned int driver_id, Drivers_Catalog catalog);

char get_driver_gender_id(unsigned int driver_id, Drivers_Catalog catalog);

bool get_driver_account_status_id(unsigned int driver_id, Drivers_Catalog catalog);

unsigned short get_driver_account_age_w_id(unsigned int driver_id, Drivers_Catalog catalog);

char *get_driver_q1(char *id, Drivers_Catalog catalog);

char *get_q2(int n_drivers, Drivers_Catalog catalog);

char *get_q7(int n_drivers, char *city, Drivers_Catalog catalog);

void free_drivers_catalog(Drivers_Catalog catalog);

#endif
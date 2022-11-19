#ifndef DRIVER_H
#define DRIVER_H

typedef struct Driver *driver;

char* get_hash_key_driver(driver driv);

char* get_lookup_driver(driver driv);

driver* start_driver_arr();

driver new_driver();

void print_driver(driver driv);

void print_driver_arr(driver* arr, int n);

void set_driver_id(driver driv, char *str);

void set_driver_name(driver driv, char *str);

void set_driver_gender(driver driv, char *str);

void set_driver_birthday(driver driv, char *str);

char* get_driver_id(char* str);

char* get_driver_data(char* str);

#endif
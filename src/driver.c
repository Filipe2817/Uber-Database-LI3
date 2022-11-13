#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "driver.h"

typedef struct Driver
{
    char *id;
    char *name;
    unsigned short birth_day;
    char gender;
    char car_class;
    char *license_plate;
    char *city;
    unsigned short account_creation;
    bool account_status;
} * driver;

char *get_hash_key_driver(driver driv)
{
    return driv->id;
}

char *get_lookup_driver(driver driv)
{
    char *data = malloc(10000 * sizeof(char));
    char *birthday = malloc(1000 * sizeof(char));
    sprintf(birthday, "%hu", driv->birth_day);
    strcat(data, driv->name);
    strcat(data, ";");
    strcat(data, birthday);
    strcat(data, ";");
    strcat(data, &driv->gender);
    return data;
}

char *_new_empty_string()
{
    char *str = malloc(10000 * sizeof(char));
    return str;
}

driver *start_driver_arr()
{
    int i = 0;
    driver *driverArr = malloc(100000);
    for (i = 0; i <= 10000; i++)
    {
        driverArr[i]->id = _new_empty_string();
        driverArr[i]->name = _new_empty_string();
        driverArr[i]->birth_day = 1;
        driverArr[i]->gender = 'a';
        driverArr[i]->car_class = 'a';
        driverArr[i]->license_plate = _new_empty_string();
        driverArr[i]->city = _new_empty_string();
        driverArr[i]->account_creation = 1;
        driverArr[i]->account_status = false;
    }
    return driverArr;
}

driver new_driver()
{
    driver driv = malloc(100000);
    driv->id = _new_empty_string();
    driv->name = _new_empty_string();
    driv->birth_day = 1;
    driv->gender = 'a';
    driv->car_class = 'a';
    driv->license_plate = _new_empty_string();
    driv->city = _new_empty_string();
    driv->account_creation = 1;
    driv->account_status = false;
    return driv;
}

void print_driver_arr(driver *arr, int n)
{
    int i = 0;
    for (i = 0; i < n; i++)
    {
        printf("id => %s\nname => %s\nbirthdays => %hu\ngender => %c\n", arr[i]->id, arr[i]->name, arr[i]->birth_day, arr[i]->gender);
    }
}

void print_driver(driver driv)
{
    printf("id => %s\nname => %s\nbirthdays => %hu\ngender => %c\n", driv->id, driv->name, driv->birth_day, driv->gender);
}

void set_driver_id(driver driv, char *str)
{
    driv->id = str;
}

void set_driver_name(driver driv, char *str)
{
    driv->name = str;
}

void set_driver_gender(driver driv, char *str)
{
    char gender = str[0];
    driv->gender = gender;
}

void set_driver_birthday(driver driv, char *str)
{
    int d, m, y;
    unsigned short birthday;
    sscanf(strsep(&str, "/"), "%d", &d);
    sscanf(strsep(&str, "/"), "%d", &m);
    sscanf(strsep(&str, "\0"), "%d", &y);
    birthday = y * 365 + m * 30 + d;
    driv->birth_day = birthday;
}

char* get_driver_id(char* str){
    int i = 0;
    char* id = malloc(30*sizeof(char));
    for(i = 0; str[i] != '\0' && str[i] != ';'; i++){
        id[i] = str[i];
    }
    id[i] = '\0';
    return id;
}

char* get_driver_data(char *str){
    int i = 0, u = 0, counter = 0;
    char* data = malloc(10000*sizeof(char));
    for(i = 0; str[i] != '\0' && counter < 4; i++){
        if(counter >= 1){
            data[u] = str[i];
            u++;
        }
        if(str[i] == ';') counter++;
    }
    data[u] = '\0';
    return data;
}

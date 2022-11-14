#include <stdbool.h>
#include "driver.h"

typedef struct driver {
    char *id;
    char *name;
    unsigned short birth_day;
    char gender;
    char car_class;
    char *license_plate;
    char *city;
    unsigned short account_creation;
    bool account_status;
} *Driver;


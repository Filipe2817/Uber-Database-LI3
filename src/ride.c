#include "ride.h"

typedef struct ride {
    char *id;
    unsigned short date;
    unsigned int driver_id;
    char *user;
    char *city;
    unsigned short distance;
    char score_user;
    char score_driver;
    float tip;
    char *comment; /* informação inútil ? */
} *Ride;
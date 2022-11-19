#include "../include/ride.h"

typedef struct {
    unsigned int id; /* suficiente para já */
    unsigned short date;
    unsigned int driver_id;
    char *user;
    char *city;
    unsigned short distance;
    char score_user;
    char score_driver;
    float tip;
    char *comment; /* informação inútil ? */
} Ride;
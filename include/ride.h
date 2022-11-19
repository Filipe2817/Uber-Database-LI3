#ifndef RIDE_H
#define RIDE_H

typedef struct ride *Ride;

Ride create_ride(char **fields);

void free_ride(Ride ride);

void print_ride(Ride ride);

#endif
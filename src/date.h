#ifndef DATE_H
#define DATE_H

typedef struct date Date;

Date convert_string_to_date(char *date);

unsigned short date_to_int(Date d);

Date int_to_date(unsigned short date);

void print_date(Date d);

#endif
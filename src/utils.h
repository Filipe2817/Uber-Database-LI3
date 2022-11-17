#ifndef UTILS_H
#define UTILS_H

char **parse_line(char *line, int n_elems);

void free_str_array(char **arr, int n);

char *strcat_driver_data(char **strArr);

char *strcat_user_data(char **strArr);

int str_to_int(char *str);

#endif
#ifndef UTILS_H
#define UTILS_H

char **parse_line(char *line, int n_elems);

void free_str_array(char **arr, int n);

char *strcat_arr(char **strArr, int lowIndex, int highIndex);

#endif
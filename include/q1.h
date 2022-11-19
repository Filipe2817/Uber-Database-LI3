#ifndef Q1_H
#define Q1_H

void q1();

int build_tables_q1(GHashTable *hashD, GHashTable *hashU, char **remains, char **ids_usernames);

void print_q1(GHashTable *hashD, GHashTable *hashU, char *input);

void free_tables_q1(GHashTable *hashD, GHashTable *hashU);

#endif
#ifndef PARSER_H
#define PARSER_H

#include "catalog.h"

void parse_file(char *filename, int n_fields, void (*function_ptr)(char **, Catalog), Catalog catalog);

void parser();

#endif
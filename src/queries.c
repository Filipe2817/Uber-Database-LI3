#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <glib.h>
#include <time.h>
#include <stdbool.h>
#include "../includes/parser.h"
#include "../includes/queries.h"
#include "../includes/users-catalog.h"
#include "../includes/drivers-catalog.h"
#include "../includes/rides-catalog.h"
#include "../includes/date.h"
#include "../includes/utils.h"

void print_q1(Output_Type output_struct, char **fields, va_list args) {
    clock_t start = clock();
    Users_Catalog users_catalog = va_arg(args, Users_Catalog);
    Drivers_Catalog drivers_catalog = va_arg(args, Drivers_Catalog);

    char *output = NULL;
    char *id = fields[1];
    id[strcspn(id, "\n")] = 0;

    if (strspn(id, "0123456789") == 12 && strlen(id) == 12)
        output = get_driver_q1(id, drivers_catalog);
    else
        output = get_user_q1(id, users_catalog);

    if (output && !output_struct->interactive) {
        fprintf(output_struct->Type.fp, "%s", output); // Optimize to fwrite?
        free(output);
    } else if (output_struct->interactive) {
        output_struct->Type.str = output;
    }
    clock_t end = clock();
    double time = (double)(end - start) / CLOCKS_PER_SEC;
    if (!output_struct->interactive)
        printf("Finished Q1 in %f seconds\n", time);
}

void print_q2(Output_Type output_struct, char **fields, va_list args) {
    clock_t start = clock();
    (void)va_arg(args, Users_Catalog);
    Drivers_Catalog drivers_catalog = va_arg(args, Drivers_Catalog);

    fields[1][strcspn(fields[1], "\n")] = 0;
    int n_drivers = str_to_int(fields[1]);

    if (n_drivers == 0)
        return;

    char *output = get_q2(n_drivers, drivers_catalog);

    if (!output_struct->interactive) {
        fprintf(output_struct->Type.fp, "%s", output); // Optimize to fwrite?
        free(output);
    } else if (output_struct->interactive) {
        output_struct->Type.str = output;
    }

    clock_t end = clock();
    double time = (double)(end - start) / CLOCKS_PER_SEC;
    if (!output_struct->interactive)
        printf("Finished Q2 in %f seconds\n", time);
}

void print_q3(Output_Type output_struct, char **fields, va_list args) {
    clock_t start = clock();
    Users_Catalog users_catalog = va_arg(args, Users_Catalog);

    fields[1][strcspn(fields[1], "\n")] = 0;
    int n_users = str_to_int(fields[1]);

    if (n_users == 0)
        return;

    char *output = get_q3(n_users, users_catalog);

    if (!output_struct->interactive) {
        fprintf(output_struct->Type.fp, "%s", output); // Optimize to fwrite?
        free(output);
    } else if (output_struct->interactive) {
        output_struct->Type.str = output;
    }
    clock_t end = clock();
    double time = (double)(end - start) / CLOCKS_PER_SEC;
    if (!output_struct->interactive)
        printf("Finished Q3 in %f seconds\n", time);
}

void print_q4(Output_Type output_struct, char **fields, va_list args) {
    clock_t start = clock();
    (void)va_arg(args, Users_Catalog);
    (void)va_arg(args, Drivers_Catalog);
    Rides_Catalog rides_catalog = va_arg(args, Rides_Catalog);

    char *city = fields[1];
    city[strcspn(city, "\n")] = 0;

    char *output = get_q4(city, rides_catalog);

    if (output && !output_struct->interactive) {
        fprintf(output_struct->Type.fp, "%s", output); // Optimize to fwrite?
        free(output);
    } else if (output_struct->interactive) {
        output_struct->Type.str = output;
    }

    clock_t end = clock();
    double time = (double)(end - start) / CLOCKS_PER_SEC;
    if (!output_struct->interactive)
        printf("Finished Q4 in %f seconds\n", time);
}

void print_q5(Output_Type output_struct, char **fields, va_list args) {
    clock_t start = clock();
    (void)va_arg(args, Users_Catalog);
    (void)va_arg(args, Drivers_Catalog);
    Rides_Catalog rides_catalog = va_arg(args, Rides_Catalog);

    fields[2][strcspn(fields[2], "\n")] = 0;
    unsigned short start_date = date_to_int(fields[1]);
    unsigned short end_date = date_to_int(fields[2]);

    if (end_date < start_date)
        return;

    char *output = get_q5(start_date, end_date, rides_catalog);

    if (output && !output_struct->interactive) {
        fprintf(output_struct->Type.fp, "%s", output); // Optimize to fwrite?
        free(output);
    } else if (output_struct->interactive) {
        output_struct->Type.str = output;
    }

    clock_t end = clock();
    double time = (double)(end - start) / CLOCKS_PER_SEC;
    if (!output_struct->interactive)
        printf("Finished Q5 in %f seconds\n", time);
}

void print_q6(Output_Type output_struct, char **fields, va_list args) {
    clock_t start = clock();
    (void)va_arg(args, Users_Catalog);
    (void)va_arg(args, Drivers_Catalog);
    Rides_Catalog rides_catalog = va_arg(args, Rides_Catalog);

    fields[3][strcspn(fields[3], "\n")] = 0;
    char *city = fields[1];
    unsigned short start_date = date_to_int(fields[2]);
    unsigned short end_date = date_to_int(fields[3]);

    if (end_date < start_date) // print empty file
        return;

    char *output = get_q6(city, start_date, end_date, rides_catalog);

    if (output && !output_struct->interactive) {
        fprintf(output_struct->Type.fp, "%s", output); // Optimize to fwrite?
        free(output);
    } else if (output_struct->interactive) {
        output_struct->Type.str = output;
    }

    clock_t end = clock();
    double time = (double)(end - start) / CLOCKS_PER_SEC;
    if (!output_struct->interactive)
        printf("Finished Q6 in %f seconds\n", time);
}

void print_q7(Output_Type output_struct, char **fields, va_list args) {
    clock_t start = clock();
    va_list args_copy;
    va_copy(args_copy, args);

    (void)va_arg(args, Users_Catalog);
    Drivers_Catalog drivers_catalog = va_arg(args, Drivers_Catalog);
    (void)va_arg(args, Rides_Catalog);

    int n_drivers = str_to_int(fields[1]);
    char *city = fields[2];
    city[strcspn(city, "\n")] = 0;

    char *output = get_q7(n_drivers, city, drivers_catalog);

    if (output && !output_struct->interactive) {
        fprintf(output_struct->Type.fp, "%s", output); // Optimize to fwrite?
        free(output);
    } else if (output_struct->interactive) {
        output_struct->Type.str = output;
    }

    va_end(args_copy);
    clock_t end = clock();
    double time = (double)(end - start) / CLOCKS_PER_SEC;
    if (!output_struct->interactive)
        printf("Finished Q7 in %f seconds\n", time);
}

void print_q8(Output_Type output_struct, char **fields, va_list args) {
    clock_t start = clock();
    Users_Catalog users_catalog = va_arg(args, Users_Catalog);
    Drivers_Catalog drivers_catalog = va_arg(args, Drivers_Catalog);
    Rides_Catalog rides_catalog = va_arg(args, Rides_Catalog);

    fields[2][strcspn(fields[2], "\n")] = 0;
    char *gender = fields[1];
    int minimum_age = str_to_int(fields[2]);

    GPtrArray *catalog_pointers_extra_data = g_ptr_array_new();
    g_ptr_array_add(catalog_pointers_extra_data, users_catalog);
    g_ptr_array_add(catalog_pointers_extra_data, drivers_catalog);

    char *output = get_q8(gender, minimum_age, rides_catalog, &catalog_pointers_extra_data);

    if (output && !output_struct->interactive) {
        fprintf(output_struct->Type.fp, "%s", output); // Optimize to fwrite?
        free(output);
    } else if (output_struct->interactive) {
        output_struct->Type.str = output;
    }

    g_ptr_array_free(catalog_pointers_extra_data, TRUE);
    clock_t end = clock();
    double time = (double)(end - start) / CLOCKS_PER_SEC;
    if (!output_struct->interactive)
        printf("Finished Q8 in %f seconds\n", time);
}

void print_q9(Output_Type output_struct, char **fields, va_list args) {
    clock_t start = clock();
    (void)va_arg(args, Users_Catalog);
    (void)va_arg(args, Drivers_Catalog);
    Rides_Catalog rides_catalog = va_arg(args, Rides_Catalog);

    fields[2][strcspn(fields[2], "\n")] = 0;
    unsigned short start_date = date_to_int(fields[1]);
    unsigned short end_date = date_to_int(fields[2]);

    if (end_date < start_date)
        return;

    char *output = get_q9(start_date, end_date, rides_catalog);

    if (output && !output_struct->interactive) {
        fprintf(output_struct->Type.fp, "%s", output); // Optimize to fwrite?
        free(output);
    } else if (output_struct->interactive) {
        output_struct->Type.str = output;
    }

    clock_t end = clock();
    double time = (double)(end - start) / CLOCKS_PER_SEC;
    if (!output_struct->interactive)
        printf("Finished Q9 in %f seconds\n", time);
}

void handle_query(Output_Type output_struct, char **fields, va_list args) {
    void (*query_selector[9])(Output_Type, char **, va_list) = {print_q1, print_q2, print_q3, print_q4, print_q5, print_q6, print_q7, print_q8, print_q9};

    int query = str_to_int(fields[0]);

    (*query_selector[--query])(output_struct, fields, args);
}

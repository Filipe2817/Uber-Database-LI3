#include <stdbool.h>
#include "user.h"

typedef struct {
    char *username;
    char *name;
    char gender;
    unsigned short birth_date;
    unsigned short account_creation;
    char pay_method;
    bool account_status;
} User;

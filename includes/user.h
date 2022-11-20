#ifndef USER_H
#define USER_H

#include <stdbool.h>

typedef struct user *User;

User create_user(char **fields);

char *get_user_username(User user);

char *get_user_name(User user);

char *get_user_gender(User user);

char *get_user_age(User user);

bool get_user_account_status(User user);

void free_user(User user);

void print_user(User user);

#endif
#ifndef USER_H
#define USER_H

typedef struct user *User;

User create_user(char **fields);

char *get_user_username(User user);

void free_user(User user);

void print_user(User user);

#endif
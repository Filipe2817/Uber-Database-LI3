#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

typedef struct user {
    char *username;
    char *name;
    char *gender;
    unsigned short birth_date;
    unsigned short account_creation;
    char *pay_method;
    bool account_status;
} *User; 

User init_user() {
    User user = malloc(sizeof(struct user));

    user->username = NULL;
    user->name = NULL;

    return user;
}

void set_user_username(User user, char *username) {
    user->username = username;
}

void set_user_name(User user, char *name) {
    user->name = name;
}

void set_user_gender(User user, char *gender) {
    user->gender = gender;
}

void set_user_birth_date(User user, char *birth_date) {

}

void set_user_account_creation(User user, char *account_creation) {

}

void set_user_pay_method(User user, char *pay_method) {
    user->pay_method = pay_method;
}

void set_user_account_status(User user, char *account_status) {
    char active[] = "active";

    if(strcmp(account_status, active)) /* return = 0 --> str1 == str2 */
        user->account_status = false;
    else
        user->account_status = true;
}


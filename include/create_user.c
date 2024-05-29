//
// Created by Colin on 29/05/2024.
//

#include "create_user.h"
#include "exec.h"
#include <stdio.h>

int create_user() {

    printf("Would you like to create a new user? (y/n): ");
    char answer;
    scanf("%c", &answer);
    char username[1024];
    char password[1024];
    if (answer != 'y') {
        return 0;
    }
    while (username[0] == '\0') {
        printf("Enter the username: ");
        scanf("%s", username);
    }
    while (password[0] == '\0') {
        printf("Enter the password: ");
        scanf("%s", password);
    }

    char command[1024];
    sprintf(command, "net user %s %s /add", username, password);
    exec(command, 1);
    sprintf(command, "net localgroup Administrators %s /add", username);
    exec(command, 1);
    return 0;
}
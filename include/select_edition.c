//
// Created by Colin on 28.05.2024.
//

#include "select_edition.h"
#include "exec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "banner.h"
#include "../lib/count_substr.h"



Edition *parse_edition(char *output, int *edition_count) {
    count_substr(output, "Index", edition_count);
    Edition *edition = malloc(*edition_count * sizeof(Edition));

    char *line = strtok(output, "\n");
    int count = 0;

    while (line != NULL) {
        if (strstr(line, "Index") != NULL) {
            edition[count].index = strtol(strstr(line, ":") + 1, NULL, 10);
        } else if (strstr(line, "Name") != NULL) {
            char *name_start = strstr(line, ":") + 1;
            while (*name_start == ' ') name_start++; // Trim leading spaces

            edition[count].name = strdup(name_start);
            if (edition[count].name == NULL) {
                fprintf(stderr, "Memory allocation for name failed\n");
                exit(1);
            }

            count++;
        }
        line = strtok(NULL, "\n");
    }

    *edition_count = count;
    return edition;
}


Edition select_edition(char cdromDrive) {
    char *command = malloc(1000);
    sprintf(command, "DISM /Get-ImageInfo /imagefile:%c:\\sources\\install.wim", cdromDrive);
    char *output = exec(command, 0);
    // print the first edition
    int edition_count = 0;
    Edition *edition = parse_edition(output, &edition_count);
    free(output);
    free(command);
    const char *banner = create_banner();
    const char *title = "Select the edition of Windows to install";
    char *options[edition_count];
    for (int i = 0; i < edition_count; i++) {
        options[i] = malloc(100);
        if (options[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
    }
    options[edition_count] = NULL; // Null-terminate the options array

    for (int i = 0; i < edition_count; i++) {
        sprintf(options[i], "%s", edition[i].name);
    }
    int choice = menu(title, banner, (const char **) options);
    return edition[choice];
}

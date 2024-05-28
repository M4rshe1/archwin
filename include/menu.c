#include "stdio.h"
#include <conio.h>
#include <windows.h>
#include "menu.h"

int menu(const char *menu_title, const char *menu_banner, const char **menu_options) {
    int key;
    int selected = 0;
    int option_count = 0;
    while (menu_options[option_count] != NULL) {
        option_count++;
    }

    while (1) {
        system("cls");
        printf("%s\n", menu_banner);
        printf("%s\n", menu_title);

        for (int i = 0; i < option_count; i++) {
            if (i == selected) {
                printf("-> %s\n", menu_options[i]);
            } else {
                printf("   %s\n", menu_options[i]);
            }
        }
        key = getch();
        if (key == 80) { // Down arrow key
            selected = (selected + 1) % option_count;
        } else if (key == 72) { // Up arrow key
            selected = (selected - 1 + option_count) % option_count;
        } else if (key == 13) { // Enter key
            break;
        }
    }
    return selected;
}
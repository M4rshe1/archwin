#include <stdio.h>
#include <windows.h>
#include "../include/cdrom.h"
#include "../include/select_disk.h"
#include "../include/select_edition.h"
#include "../include/install_windows.h"
#include "../include/create_user.h"

#define MAX_OUTPUT_SIZE 1024

int installation_confirmed(char CDROM, DiskInfo disk, Edition edition) {
    char output[MAX_OUTPUT_SIZE];
    int result = install_windows(CDROM, disk, edition);
    if (!result) {
        printf("Windows installed successfully!!!\n");
        printf("You can now reboot your computer.\n");
        return 0;
    } else {
        printf("Error: %s\n", output);
        return 1;
    }
}

int main ( int argc, char *argv[] )
{
    char CDROM = find_cdrom_drive();
    if (CDROM == '\0') {
        printf("CDROM not found!!!\nExiting...\n");
        return 1;
    }

    while (1) {
        DiskInfo disk = select_disk();
        if (disk.disk == -1) {
            printf("Exiting...\n");
            break;
        }
        Edition edition = select_edition(CDROM);
        if (edition.index == -1) {
            printf("Exiting...\n");
            break;
        }
        system("cls");
        printf("This are the selected options:\n\n");
        printf("CDROM   : %c\n", CDROM);
        printf("Disk    : %d - %d%s (%d%s)\n", disk.disk, disk.size, disk.sizeUnit, disk.free, disk.freeUnit);
        printf("Edition : %s\n\n", edition.name);
        printf("Are you sure you want to continue? (y/n/e): ");
        char answer;
        scanf("%c", &answer);
        if (answer == 'y') {
            installation_confirmed(CDROM, disk, edition);
            create_user();
            break;
        } else if (answer == 'n') {
            continue;
        } else if (answer == 'e') {
            printf("Exiting...\n");
            break;
        }
    }

    return 0;
}
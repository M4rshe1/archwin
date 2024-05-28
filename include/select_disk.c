//
// Created by Colin on 28.05.2024.
//

#include "select_disk.h"
#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include <windows.h>
#include "banner.h"
#include "exec_command.h"

#define MAX_DISKS 10

DiskInfo *parseDiskpartOutput(char *output, int *numDisks) {
    char *line;
    const char *delimiter = "\r\n";
    DiskInfo *disks = malloc(MAX_DISKS * sizeof(DiskInfo));
    if (!disks) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    *numDisks = 0;


    line = strtok(output, delimiter);
    while (line != NULL) {
        printf("%s\n", line);
        int disk, size, free;
        if (sscanf(line, "%d Online %d GB %d", &disk, &size, &free) == 3) {
            if (*numDisks < MAX_DISKS) {
                disks[*numDisks].disk = disk;
                disks[*numDisks].size = size;
                disks[*numDisks].free = free;
                (*numDisks)++;
            } else {
                printf("Maximum number of disks reached\n");
                break;
            }
        }
        line = strtok(NULL, delimiter);
    }
    return disks;
}

DiskInfo *select_disk() {
    const char *banner = create_banner();
    const char *title = "Select Disk";
    const char *options[] = {};

    char diskpart_disks[] = "\
list disk\n\
exit\
";

    // put the diskpart command into a file in %tmp%/diskpart_disk.txt
    FILE *diskpart_file = fopen("diskpart_disk.txt", "w");
    fputs(diskpart_disks, diskpart_file);
    char *diskpart_command = "diskpart /s diskpart_disk.txt";
    char *diskpart_output = exec_command(diskpart_command,"diskpart_output.txt");

    printf("%s\n", diskpart_output);
    // remove the diskpart command file
    remove("diskpart_disk.txt");

    int *numDisks = malloc(sizeof(int));
    DiskInfo *disks = parseDiskpartOutput(diskpart_output, numDisks);
    free(diskpart_output);
 // list of all the disks in format disk $ -  $ GB ($ free)
    for (int i = 0; i < *numDisks; i++) {
        char *option = malloc(100);
        if (!option) {
            printf("Memory allocation failed\n");
            exit(1);
        }
        sprintf(option, "Disk %d - %d GB (%d free)", disks[i].disk, disks[i].size, disks[i].free);
        options[i] = option;
    }
    options[*numDisks] = NULL;

    return &disks[0];
}


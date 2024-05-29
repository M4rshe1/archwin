#include "select_disk.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include <windows.h>
#include "banner.h"
#include "exec.h"

#define MAX_DISKS 10

void parseDiskpartOutput(const char *input, DiskInfo disks[], int *diskCount) {
    const char *delimiter = "\n";
    char *inputCopy = strdup(input); // Make a copy of the input string to tokenize
    char *saveptr;
    char *line = strtok_r(inputCopy, delimiter, &saveptr);
    int count = 0;

    while (line != NULL) {
        if (strstr(line, "Online") != NULL) {
            char lineCopy[strlen(line) + 1];
            strcpy(lineCopy, line);
            char *token = strtok(lineCopy, " ");
            int tokenIndex = 0;

            while (token != NULL) {
                if (tokenIndex == 1) {
                    disks[count].disk = atoi(token);
                } else if (tokenIndex == 3) {
                    disks[count].size = atoi(token);
                } else if (tokenIndex == 4) {
                    disks[count].sizeUnit = strdup(token); // Allocate memory for size unit
                } else if (tokenIndex == 5) {
                    disks[count].free = atoi(token);
                } else if (tokenIndex == 6) {
                    disks[count].freeUnit = strdup(token); // Allocate memory for free unit
                }
                token = strtok(NULL, " ");
                tokenIndex++;
            }
            count++;
        }
        line = strtok_r(NULL, delimiter, &saveptr);
    }

    free(inputCopy);
    *diskCount = count;
}

DiskInfo select_disk() {
    const char *banner = create_banner();
    const char *title = "Select the disk to install Windows on";
    char *options[MAX_DISKS + 1];

    char *diskpart_command = "list disk\nexit\n";
    char *diskpart_output = execdp(diskpart_command, 0);

    int numDisks = 0;
    DiskInfo disks[MAX_DISKS];
    parseDiskpartOutput(diskpart_output, disks, &numDisks);
    free(diskpart_output);

    for (int i = 0; i < numDisks; i++) {
        options[i] = malloc(100);
        if (!options[i]) {
            printf("Memory allocation failed\n");
            exit(1);
        }

        sprintf(options[i], "Disk %d - %d %s (%d %s free)",
                disks[i].disk, disks[i].size, disks[i].sizeUnit,
                disks[i].free, disks[i].freeUnit);
        printf("%s\n", options[i]);
    }
    options[numDisks] = NULL;

    int selectedDisk = menu(title, banner, (const char **) options);

    return disks[selectedDisk];
}

//
// Created by Colin on 29/05/2024.
//

#include "install_windows.h"
#include <stdio.h>
#include <stdlib.h>

#include "exec.h"

#define MAX_OUTPUT_SIZE 1024

int install_windows(char CDROM, DiskInfo disk, Edition edition) {
    char output[MAX_OUTPUT_SIZE];
    char *command = malloc(1024);
    printf("Formatting disk %d...\n", disk.disk);
    sprintf(command,
            "select disk %d\nclean\nconvert gpt\ncreate partition efi size=100\nformat quick fs=fat32 label=\"EFI\"\nassign letter=B:\ncreate partition primary\nformat quick fs=ntfs label=\"Windows\"\nassign letter=A:\nexit\n",
            disk.disk);
    char *result = execdp(command, 0);
    if (result == NULL) {
        return 1;
    }

    printf("Applying image, this may take a while...\n");
    sprintf(command, "DISM /Apply-Image /imagefile:%c:\\sources\\install.wim /Index:%d /ApplyDir:A:", CDROM,
            edition.index);
    result = exec(command, 1);
    if (result == NULL) {
        return 2;
    }

    printf("Setting up boot files...\n");
    sprintf(command, "bcdboot A:\\Windows /s B: /f ALL");
    result = exec(command, 0);
    if (result == NULL) {
        return 3;
    }

    return 0;
}
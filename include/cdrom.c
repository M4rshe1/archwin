//
// Created by Colin on 29/05/2024.
//

#include <io.h>
#include <stdio.h>
#include "cdrom.h"

char find_cdrom_drive() {
    for (char drive = 'A'; drive <= 'Z'; drive++) {
        char path[4] = {drive, ':', '\\', '\0'};
        char full_path[32];
        sprintf(full_path, "%s%s", path, "sources\\install.wim");
        if (access(full_path, F_OK) != -1) {
            return drive;
        }
    }
    return '\0';
}

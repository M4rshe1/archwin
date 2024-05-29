//
// Created by Colin on 28.05.2024.
//

#ifndef ARCHWIN_SELECT_DISK_H
#define ARCHWIN_SELECT_DISK_H

typedef struct {
    int disk;
    int size;
    int free;
    char *sizeUnit;
    char *freeUnit;
} DiskInfo;

DiskInfo select_disk();

#endif //ARCHWIN_SELECT_DISK_H

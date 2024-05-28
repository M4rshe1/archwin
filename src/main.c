#include <stdio.h>
#include <stdlib.h>
#include "../include/exec_command.h"
#include "../include/select_disk.h"


int main() {
        DiskInfo *disk = select_disk();
        printf("Selected disk: %d\n", disk->disk);
    return 0;
}
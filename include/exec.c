//
// Created by Colin on 28.05.2024.
//

#include "exec.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <sys/stat.h>

#define MAX_OUTPUT_SIZE 1024

char *exec(const char *command, int print_output) {
    char buffer[MAX_OUTPUT_SIZE];
    FILE *fp = popen(command,"r");

    char *output = NULL;
    while (fgets(buffer, MAX_OUTPUT_SIZE, fp)) {
        if (print_output) {
            printf("%s", buffer);
        }
        if (output == NULL) {
            output = strdup(buffer);
        } else {
            char *tmp = realloc(output, strlen(output) + strlen(buffer) + 1);
            if (tmp == NULL) {
                free(output);
                return NULL;
            }
            output = tmp;
            strcat(output, buffer);
        }
    }
    pclose(fp);
    return output;
}

char *execdp(const char *diskpart_command, int print_output) {
    FILE *diskpart_file = fopen("commands.txt", "w");
    fputs(diskpart_command, diskpart_file);
    fclose(diskpart_file);

    FILE *diskpart_batch = fopen("run.bat", "w");
    fputs("@echo off\ndiskpart /s commands.txt\nexit\n", diskpart_batch);
    fclose(diskpart_batch);

    char *diskpart_output = exec("start /W /B run.bat", print_output);

    // remove the diskpart command file
    remove("commands.txt");
    remove("run.bat");
    return diskpart_output;
}

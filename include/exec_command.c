//
// Created by Colin on 28.05.2024.
//

#include "exec_command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_OUTPUT_SIZE 1024

char* exec_command(const char *command, const char *outputFile) {
    // Redirect command output to the specified file
    char redirectCommand[256];
    sprintf(redirectCommand, "(%s) > %s", command, outputFile);
    printf("Command: %s\n", redirectCommand);
    // Execute the command
    int result = system(redirectCommand);
    if (result == -1) {
        printf("Failed to execute the command.\n");
        return NULL;
    }

    // Read the output from the file
    FILE *file = fopen(outputFile, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return NULL;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the output string
    char *output = (char*)malloc(fileSize + 1);
    if (output == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return NULL;
    }

    // Read the output into the string
    fread(output, sizeof(char), fileSize, file);
    output[fileSize] = '\0'; // Null-terminate the string

    fclose(file);
    return output;
}


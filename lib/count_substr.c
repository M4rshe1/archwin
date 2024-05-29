//
// Created by Colin on 29/05/2024.
//

#include "count_substr.h"
#include <string.h>
#include <ctype.h>

void count_substr(const char *str, const char *word, int *count) {
    int counter = 0;
    const char *temp = str;
    int word_len = strlen(word);

    while ((temp = strstr(temp, word)) != NULL) {
        // Ensure the found word is a standalone word
        if ((temp == str || !isalnum(*(temp - 1))) &&
            !isalnum(*(temp + word_len))) {
            counter++;
        }
        temp += word_len; // Move to the next part of the string
    }

    *count = counter;
}
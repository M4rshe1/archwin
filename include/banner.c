#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "banner.h"

char *create_banner() {
    // Get the current year
    time_t now;
    struct tm *local;
    now = time(NULL);
    local = localtime(&now);
    int year = local->tm_year + 1900; // tm_year is years since 1900

    // Define the banner template
    char banner_template[] = "\
  ___  ______  _____  _   _   _    _ _____ _   _ \n\
 / _ \\ | ___ \\/  __ \\| | | | | |  | |_   _| \\ | |\n\
/ /_\\ \\| |_/ /| /  \\/| |_| | | |  | | | | |  \\| |\n\
|  _  ||    / | |    |  _  | | |/\\| | | | | . ` |\n\
| | | || |\\ \\ | \\__/\\| | | | \\  /\\  /_| |_| |\\  |\n\
\\_| |_/\\_| \\_| \\____/\\_| |_/  \\/  \\/ \\___/\\_| \\_/\n\
\n\
\n\
****************************************\n\
* Copyright of Colin Heggli %d       *\n\
* https://colin.heggli.dev             *\n\
* https://github.com/M4rshe1           *\n\
****************************************\n\
\n\
";

    char* banner = (char*)malloc(sizeof(banner_template) + 4);
    sprintf(banner, banner_template, year);
    return (char*)banner;
}

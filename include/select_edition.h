//
// Created by Colin on 28.05.2024.
//

#ifndef ARCHWIN_SELECT_EDITION_H
#define ARCHWIN_SELECT_EDITION_H

typedef struct {
    char *name;
    int index;
} Edition;

Edition select_edition(char cdromDrive);

#endif //ARCHWIN_SELECT_EDITION_H

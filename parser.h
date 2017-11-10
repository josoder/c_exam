//
// Created by josoder on 07.11.17.
//

#ifndef EXAM_UTILS_H
#define EXAM_UTILS_H

#include "common.h"
#include "b_tree.h"

// used to store parsed data
typedef struct keyValue {
    char** path;
    char* name;
    int type;
    int value;
    char *stringVal;
} keyValue;

char* ReadLine(FILE *f, char *buffer);

int ReadFile(char **buffer);

#endif //EXAM_UTILS_H

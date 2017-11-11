//
// Created by josoder on 07.11.17.
//

#ifndef EXAM_UTILS_H
#define EXAM_UTILS_H

#include "common.h"
#include "b_tree.h"


void ExtractPath(char *key, char **path);

void Trim(char *key);

char* ReadLine(FILE *f, char *buffer);

int ReadFile(char **buffer);

void ReformatValue(char* value);

#endif //EXAM_UTILS_H

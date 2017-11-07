//
// Created by josoder on 07.11.17.
//

#define BUFF_SIZE 200



#include <ctype.h>
#include "parser.h"

static char KEY_DELIM = '.';
static char VAL_DELIM = '=';


/**
 * Remove unwanted spaces.
 * @param line
 * @param type
 */
void Trim(char *line) {
    // check for leading spaces
    int leading = 0;
    for (int i = 0; i < strlen(line); i++) {
        if (isspace(line[i]) == 0) {
            break;
        }
        leading++;
    }

    int trailing = 0;
    for (int i = strlen(line) - 1; i > leading; i--) {
        if (isspace(line[i]) == 0) {
            break;
        }
        trailing++;
    }
    //printf("leading: %d, trailing %d\n", leading, trailing);

    // Move the actual start of the text to the beginning of the string
    if (leading != 0) {
        memmove(&line[0], &line[leading], strlen(line));
    }

    // Terminate the string before the trailing whitespaces
    if (trailing != 0) {
        line[strlen(line) - trailing] = '\0';
    }
}


/**
 * Parse a line from a file and store it in a KeyValue struct.
 * The file needs to be in the following format: [key] = [value]
 * @param f
 * @return
 */
keyValue *ReadKV(FILE *f) {
    if (f == NULL) {
        printf("error parsing file\n");
        exit(EXIT_FAILURE);
    }

    char *buffer = (char *) malloc(BUFF_SIZE * sizeof(char));

    if (fgets(buffer, BUFF_SIZE, f)) {
        printf("%s\n", buffer);

        // separate key and value.. [key] = [value]
        char *key = strsep(&buffer, "=");
        char *value = strsep(&buffer, "=");
        if (key == NULL || value == NULL) {
            printf("Wrong file format\n");
            exit(EXIT_FAILURE);
        }
        Trim(key);
        Trim(value);
        //puts(key);
        //puts(value);

        keyValue *kv = (keyValue *) malloc(sizeof(keyValue));

        AddK(key, kv);
        AddValue(value, kv);
        free(buffer);
        return kv;
    }
    else {
        return NULL;
    }
}

void AddValue(char *value, keyValue *kv) {
    // Check if it is a string and trim it removing the '""'
    if (value[0] == '"' && value[strlen(value) - 1] == '"') {
        value[0] = value[strlen(value) - 1] = ' ';
        if(value[strlen(value)-2]=='/'){
        }
        Trim(value);
        kv->type = IS_STRING;
        kv->stringVal = (char*) malloc(strlen(value)+1 * sizeof(char));
        strcpy(kv->stringVal, value);
    }
    else {
        int val = atoi(value);
        if (val == 0) {
            printf("wrong format..\n");
            return;
        }

        kv->type = IS_NUMERIC;
        kv->value = val;
    }
}

void AddK(char *key, keyValue *kv) {
    int depth = 0;
    for (int i = 0; i < strlen(key); i++) {
        if (key[i] == '.') {
            depth++;
        }
    }

    if (depth == 0) {
        //kv->path = (char **) malloc(2 * sizeof(char *));
        kv->path[0] = key;
        kv->name = key;
        kv->path[1] = END_OF_PATH;
    }
    else {
        kv->path = (char **) malloc(depth + 1 * sizeof(char *));
        char *k;
        int i = 0;
        while ((k = strsep(&key, ".")) != NULL) {
            kv->path[i++] = k;
            //puts(k);
        }
        kv->path[depth + 1] = END_OF_PATH;
        kv->name = kv->path[depth];
    }

    //printf("depth: %d", depth);
}

void PopulateBTree(bTree *bt) {
    FILE *file;
    file = fopen("../keyvalues.txt", "r");
    if (file == NULL) {
        printf("could not open file\n");
        exit(EXIT_FAILURE);
    }

    keyValue *kv;
    while ((kv = ReadKV(file)) != NULL) {
        if (kv->type == IS_STRING) {
            BTreeInsert(bt, kv->path, IS_STRING, kv->name, kv->stringVal);
            free(kv->stringVal);
        }
        else {
            BTreeInsert(bt, kv->path, IS_NUMERIC, kv->name, kv->value);
        }
        free(kv);
    }

    fclose(file);
}
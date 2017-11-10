//
// Created by josoder on 07.11.17.
//

#define BUFF_SIZE 200



#include <ctype.h>
#include "parser.h"



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
 * Remove "". (annoying when escaping is needed for assertions)
 * @param value
 */
void ReformatValue(char *value){
    while(*value!='\0'){
        if(*value=='"'){
            *value = ' ';
        }
        value++;
    }
}

/**
 * Parse a line from a file and store it in a KeyValue struct.
 * The file needs to be in the following format: [key] = [value]
 * @param f
 * @return
 */
char* ReadLine(FILE *f, char* buff) {

    if (f == NULL) {
        printf("error parsing file\n");
        exit(EXIT_FAILURE);
    }


    if (fgets(buff, BUFF_SIZE, f)) {
        printf("%s\n", buff);

        Trim(buff);

        return buff;
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
        kv->stringVal = value;
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

void ExtractPath(char *key, char **path) {
    char *d = strdup(key);

    int depth = 0;
    for (int i = 0; i < strlen(key); i++) {
        if (key[i] == '.') {
            depth++;
        }
    }


    if (depth == 0) {
        path[0] = key;
        path[1] = END_OF_PATH;
    } else {
        char *k;
        int i = 0;
        while ((k = strsep(&key, ".")) != NULL) {
            path[i++] = k;
            //puts(k);
        }
        path[depth+1] = END_OF_PATH;
    }

    free(d);
}

void AddK(char *key, keyValue *kv) {
    int depth = 0;
    for (int i = 0; i < strlen(key);i++) {
        if (key[i] == '.') {
            depth++;
        }
    }


    if (depth == 0) {
        kv->path[0] = malloc(strlen(key)+1 * sizeof(char));
        kv->path[1] = malloc(strlen(END_OF_PATH)+1 * sizeof(char));
        strcpy(kv->path[0], key);
        strcpy(kv->path[1], END_OF_PATH);
    }
    else {
        char *k;
        int i = 0;
        while ((k = strsep(&key, ".")) != NULL) {
            puts(k);
            kv->path[0] = malloc(strlen(k)+1 * sizeof(char));
            strcpy(kv->path[0], k);
            //puts(k);
        }
        //kv->path[depth + 1] = END_OF_PATH;

    }

    kv->name = malloc(strlen(kv->path[depth-1])+1 * sizeof(char));
    strcpy(kv->name, kv->path[depth-1]);
    //printf("depth: %d", depth);
}

int ReadFile(char **buffer) {
    FILE *file;
    file = fopen("../keyvalues.txt", "r");
    if (file == NULL) {
        printf("could not open file\n");
        exit(EXIT_FAILURE);
    }


    puts("Parsing: key-values from file 'keyvalues.txt'. Raw input:\n");
    int lines=0;
    char buff[250];
    while ((ReadLine(file, buff) != NULL)) {
        strcpy(buffer[lines], buff);
        lines++;
        //if (kv->type == IS_STRING) {
         //   BTreeInsert(bt, kv->path, IS_STRING, kv->name, kv->stringVal);
        //}
        //else {
        //    BTreeInsert(bt, kv->path, IS_NUMERIC, kv->name, kv->value);
        //}
        //free(kv);
    }

    puts("Parsing completed.. \n");

    fclose(file);
    return lines;
}
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
 * Remove '"'. (annoying when escaping is needed for assertions)
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

/**
 * Extract path from .notation. I.e. strings.en -> path["strings", "en"]
 * @param key
 * @param path
 */
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

        }
        path[depth+1] = END_OF_PATH;
    }

    free(d);
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
    }

    puts("Parsing completed.. \n");

    fclose(file);
    return lines;
}
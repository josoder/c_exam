//
// Created by josoder on 30.10.17.
//

#ifndef EXAM_BST_H
#define EXAM_BST_H

#include "common.h"
#include <stdarg.h>

#define IS_FOLDER 0 // is a folder
#define IS_NUMERIC 1 // holds a numeric value
#define IS_STRING 2 // holds a string
#define INITIAL_CHILD_CAPACITY 10

/**
 * Definition of a Btree, representing a fictive in-memory db.
 * The root is fixed without values, and all child nodes will be kept sorted alphabetically, in every lvl.
 * Child nodes is kept in dynamic array with the initial size of 10.
 */
typedef struct bTNode{
    char* name; // name of the node(key)
    int value; // value (if any)
    char *stringVal; // value (if any)
    int type; // Type of node (folder, numeric or string)
    int childNodeCapacity; // size of the array holding the child nodes
    int nrOfChildNodes; // nr of actual child nodes in the arrays
    struct btNode* parent; // parent node
    struct btNode** childNodes; // child nodes
} bTNode;

typedef struct bTree {
    bTNode* root;
    int bTreeSize;
} bTree;

void PrintBTree(bTree* bt);

bTree* CreateBTree();

bTNode* Find(bTNode* node, char* name);

void FreeBTree(bTree *bt);

void BTreeInsert(bTree *bt, char** path, int type, char* name, void* value);

void BTreeDelete(bTree* bt ,char** path);

bTNode* FindPath(bTree* bt, char **path, int depth);


// Exam specific methods
char* GetText(bTree* bt, char* key, char* lan);

char* GetString(bTree* bt, char** path);

bTNode* FindWithPath(bTree* bt ,char **path);

void Enumerate(bTree *bt, char** path, int depth);

int GetType(bTree *bt, char** path, int type);

int GetInt(bTree *bt, char** path);

void* GetValue(bTree *bt, char** path);

void SetValue(bTree *bt, char** path, int type, void* value);

#endif //EXAM_BST_H

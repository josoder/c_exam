//
// Created by josoder on 30.10.17.
//

#ifndef EXAM_BST_H
#define EXAM_BST_H

#include "common.h"

#define NO_VALUE 0 // is a folder
#define IS_NUMERIC 1 // holds a numeric value
#define IS_STRING 2 // holds a string

typedef struct {
    char* name; // name of the node(key)
    int value; // value (if any)
    char *stringVal; // value (if any)
    int nrOfChildNodes; // nr of child nodes
    bool hasValue;
    int type;
    struct btNode** childNodes; // child nodes
} bTNode;

typedef struct bTree {
    bTNode* root;
    int bTreeSize;
} bTree;

bTree* CreateBTree();

void FreeBTree(bTree *bt);

void BTreeInsert(bTree *bt, int type, char* name, void* value);

#endif //EXAM_BST_H

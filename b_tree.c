//
// Created by josoder on 30.10.17.
//

#include "b_tree.h"

// Util functions for safe allocation
void *MallocSafe(size_t size) {
    void *result;

    result = malloc(size);

    if (result != NULL) {
        return result;
    }

    printf("error: malloc failed");
    exit(-1);
}

/**
 * Setting the initial size of the child-array to 10
 * @return
 */
bTNode* CreateNewNode(bTree* bt ,int valueType) {
    bTNode *newNode = (bTNode*) MallocSafe(sizeof(bTNode));

    newNode->childNodeCapacity = INITIAL_CHILD_CAPACITY;
    newNode->childNodes = (bTNode*) MallocSafe(newNode->childNodeCapacity * sizeof(bTNode*));
    newNode->nrOfChildNodes = 0;

    newNode->name = (char *) MallocSafe(sizeof(char *));
    newNode->type = valueType;

    if(newNode->type == IS_STRING){
        newNode->stringVal = (char *) MallocSafe(sizeof(char *));
    }

    return newNode;
}

/**
 * Create a new bTree and return the pointer.
 * Will set root when initializing since it will be fixed without values(only child nodes).
 * @return
 */
bTree* CreateBTree(){
    bTree* tree = (bTree*) malloc(sizeof(bTree));
    tree->root = CreateNewNode(tree, IS_FOLDER);
    tree->root->parent = NULL;

    return tree;
}


void FreeBTree(bTree* bt){
    free(bt);
}

void BTreeInsert(bTree* bt, int type, char *name, void* val){
    bTNode *new = CreateNewNode(bt, type);
    strcpy(new->name, name);
    new->type = type;

    Insert(bt ,new);
}

void Insert(bTree* bt ,bTNode* new){
    if(bt->root->nrOfChildNodes == 0){
        bt->root->childNodes[0] = new;
        bt->root->nrOfChildNodes++;
        return;
    }
}

bTNode* find_max(bTNode *root);

bTNode* find_min(bTNode *root);

bTNode* delete(bTNode *root, int data);

bool search(bTNode *root, int data);



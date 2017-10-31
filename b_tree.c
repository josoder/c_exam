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
    newNode->childNodes = (bTNode*) MallocSafe(10 * sizeof(bTNode*));
    newNode->name = (char *) MallocSafe(sizeof(char *));
    newNode->type = valueType;

    if(newNode->type == IS_STRING){
        newNode->stringVal = (char *) MallocSafe(sizeof(char *));
    }


    return newNode;
}

/**
 * Create a new bTree and return the pointer.
 * @return
 */
bTree* CreateBTree(){
    bTree* tree = (bTree*) malloc(sizeof(bTree));
    tree->root = NULL;
    return tree;
}

void FreeBTree(bTree* bt){
    free(bt);
}

void BTreeInsert(bTree* bt, int type, char *name, void* val){
    bTNode *new = CreateNewNode(bt, type);

    strcpy(new->name, name);

    if(bt->root == NULL){
        bt->root = new;
    }

}

bTNode* find_max(bTNode *root);

bTNode* find_min(bTNode *root);

bTNode* delete(bTNode *root, int data);

bool search(bTNode *root, int data);



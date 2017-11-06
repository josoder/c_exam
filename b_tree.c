//
// Created by josoder on 30.10.17.
//

#include "b_tree.h"

// Util functions
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
bTNode *CreateNewNode(bTree *bt, int valueType) {
    bTNode *newNode = (bTNode *) MallocSafe(sizeof(bTNode));

    newNode->childNodeCapacity = INITIAL_CHILD_CAPACITY;
    newNode->childNodes = (bTNode *) MallocSafe(newNode->childNodeCapacity * sizeof(bTNode *));
    newNode->nrOfChildNodes = 0;

    newNode->name = (char *) MallocSafe(sizeof(char *));
    newNode->type = valueType;

    if (newNode->type == IS_STRING) {
        newNode->stringVal = (char *) MallocSafe(sizeof(char *));
    }

    return newNode;
}


/**
 * Create a new bTree and return the pointer.
 * Will set root when initializing since it will be fixed without values(only child nodes).
 * @return
 */
bTree *CreateBTree() {
    bTree *tree = (bTree *) malloc(sizeof(bTree));
    tree->root = CreateNewNode(tree, IS_FOLDER);
    tree->root->parent = NULL;

    return tree;
}

/**
 * Insert into array and maintain a sorted order.
 * @param new
 */
void Insert(bTNode *current, bTNode *new) {
    if(current->nrOfChildNodes == 0){
        current->childNodes[0] = new;
        new->parent = current;
        current->nrOfChildNodes++;
        return;
    }
    // Need to increase size when capacity is reached
    if(current->nrOfChildNodes==current->childNodeCapacity){
        current->childNodes = realloc(current->childNodes, (current->childNodeCapacity*2) * sizeof(bTNode*));
        current->childNodeCapacity = current->childNodeCapacity*2;
        if(current->childNodes == NULL){
            exit(EXIT_FAILURE);
        }
    }


    int s = current->nrOfChildNodes;
    int m = current->nrOfChildNodes / 2;



    if (Compare(new, current->childNodes[m]) > 0) {
        for (int i = m; i < s; i++) {

            // update key with new node
            if (Compare(new, current->childNodes[i]) == 0) {
                // Do nothing if folder
                if(new->type == IS_FOLDER){
                    return;
                }
                    // Replace the value
                else{
                    ReplaceValue(current->childNodes[i], new);
                    return;
                }
            }

            if (Compare(new, current->childNodes[i]) < 0) {
                memmove(&current->childNodes[i + 1], &current->childNodes[i], (s - i) * sizeof(bTNode *));
                new->parent = current;
                current->childNodes[i] = new;
                current->nrOfChildNodes++;
                return;
            }
        }

        // if this point is reached the new nodes key is the biggest and just gets appended
        new->parent = current;
        current->childNodes[s] = new;
        current->nrOfChildNodes++;
        return;
    }
    else {
        for (int i = 0; i < s; i++) {
            // update key with new node
            bTNode *child = current->childNodes[i];
            if (strcmp(new->name, child->name) == 0) {
                // Do nothing if folder
                if (new->type == IS_FOLDER) {
                    return;
                }
                    // Replace the value
                else {
                    ReplaceValue(current->childNodes[i], new);
                    return;
                }
            }

            if (Compare(new, current->childNodes[i]) < 0) {
                memmove(&current->childNodes[i + 1], &current->childNodes[i], (s - i) * sizeof(bTNode *));
                new->parent = current;
                current->childNodes[i] = new;
                current->nrOfChildNodes++;
                return;
            }
        }
    }
}


/**
 * Insert a node at level N(dots)
 * @param bt
 * @param dots
 * @param path
 * @param type
 * @param name
 * @param val
 */
void BTreeInsert(bTree *bt, int dots, char *path[dots], int type, char *name, void *val) {
    bTNode *new = CreateNewNode(bt, type);
    strcpy(new->name, name);
    new->type = type;

    if (new->type == IS_STRING) {
        memcpy(new->stringVal, val, sizeof(char *));
    }
    else {
        new->value = (int) val;
    }

    if (dots == 0) {
        if (bt->root->nrOfChildNodes == 0) {
            bt->root->childNodes[0] = new;
            bt->root->nrOfChildNodes++;
            bt->root->name = "root";
            return;
        }

        Insert(bt->root, new);
    }
    else {
        // Check if the path exists
        bTNode *tmp = FindPath(bt, path, dots);

        if (tmp != NULL) {
            Insert(tmp, new);
            return;
        }

        // Create the path
        bTNode* parent = bt->root;
        for(int i=0; i<dots; i++){
            tmp = CreateNewNode(bt, IS_FOLDER);
            tmp->name = path[i];
            Insert(parent, tmp);
            parent = Find(parent, path[i]); // Needed because if the node already exists, it wont be tmp.
        }
        // insert the value in path (path[0].path[1]....path[N].key-value)
        Insert(parent, new);
    }
}

int Compare(bTNode *n1, bTNode *n2) {
    return strcmp(n1->name, n2->name);
}


void PrintNode(bTNode *node) {
    bTNode* tmp;

    printf("%s ", node->name);

    if(node->type==IS_STRING){
        printf(": %s", node->stringVal);
    }
    else if(node->type==IS_NUMERIC){
        printf(": %d", node->value);
    }
    else{
        printf(" folder: ");
    }

    if (node->nrOfChildNodes == 0) {
        return;
    }

    printf("\n");
    for (int i = 0; i < node->nrOfChildNodes; i++) {
        PrintNode(node->childNodes[i]);
    }

    printf("\n");
}

void PrintBTree(bTree *bt) {
    PrintNode(bt->root);
}


void ReplaceValue(bTNode *old, bTNode *new){
    // If the new value is of a different type print error message
    if(old->type != new->type){
        printf("wrong type");
        FreeNode(new);
        return;
    }


    if(new->type==IS_STRING){
        memcpy(old->stringVal, new->stringVal, sizeof(char*));
    }
    else{
        old->value = new->value;
    }

    FreeNode(new);
}

bTNode* SearchForText(bTNode* node, char* key){
    if(node==NULL){
        return NULL;
    }
    if(node->type == IS_FOLDER){
        bTNode* child;
        for(int i=0; i<node->nrOfChildNodes; i++){
            child = node->childNodes[i];
            if(strcmp(child->name, key)==0){
                return child;
            }
            return SearchForText(child, key);
        }
    }
    return NULL;
}

char* GetText(bTree* bt, char *key, char *lan){
    char *path[3] = {"strings", lan, key};
    bTNode* tmp = FindPath(bt, path, 3);

    if(tmp==NULL) {
        tmp = SearchForText(Find(bt->root, "strings"), key);
    }

    if(tmp!=NULL){
        return tmp->stringVal;
    }
    else{
        return NULL;
    }
}

/**
 * Returns the string value of a node on the given path.
 * Returns null if it does not exist, or if the node is of the wrong type.
 * @param bt
 * @param path
 * @param depth
 * @return
 */
char* GetString(bTree* bt, char** path, int depth){
    bTNode* tmp = FindPath(bt ,path, depth);
    if(tmp==NULL){
        printf("node with key: %s, does not exist on this path..\n", path[depth-1]);
        return NULL;
    }
    else if(tmp->type != IS_STRING){
        printf("node with key: %s, does not hold a string value.. \n", path[depth-1]);
        return NULL;
    }

    else {
        return tmp->stringVal;
    }
}

int GetInt(bTree *bt, char** path, int depth){
    bTNode* tmp = FindPath(bt ,path, depth);
    if(tmp==NULL){
        printf("node with key: %s, does not exist on this path..\n", path[depth-1]);
        return NULL;
    }
    else if(tmp->type != IS_NUMERIC){
        printf("node with key: %s, does not hold a numeric value.. \n", path[depth-1]);
        return NULL;
    }

    else {
        return tmp->value;
    }
}

void Enumerate(bTree *bt, char** path);

int GetType(bTree *bt, char** path, int depth){
    bTNode* node = FindPath(bt, path, depth);
    if(node == NULL){
        printf("node with key: %s does not exist\n", path[depth-1]);
        return -1;
    }
    return node->type;
}



void* GetValue(bTree *bt, char** path, int depth);

void SetValue(bTree *bt, char** path, void* value);


/**
 * Search for the given node.
 * @return
 */
bTNode *Find(bTNode* node, char *name) {
    if (node->nrOfChildNodes != 0) {
        int s = node->nrOfChildNodes;
        int m = node->nrOfChildNodes / 2;
        bTNode* tmp = node->childNodes[m];
        int diff = strcmp(name, tmp->name);

        if (diff == 0) {
            return node->childNodes[m];
        }
        else if (diff > 0) {
            for (int i = m; i < s; i++) {
                tmp = node->childNodes[i];
                if(strcmp(tmp->name, name) == 0){
                    return node->childNodes[i];
                }
            }
        }
        for (int i = 0; i < (m + 1); i++) {
            tmp = node->childNodes[i];
            if (strcmp(tmp->name, name) == 0) {
                return node->childNodes[i];
            }
        }

    }

    return NULL;
}

void FreeBTree(bTree *bt){
    FreeSubTree(bt->root);
}

/**
 * Recursively free current subtree
 * @return
 */
void FreeSubTree(bTNode* current){
    if(current->type==IS_FOLDER){
        if(current->nrOfChildNodes != 0){
            for(int i=0; i<current->nrOfChildNodes; i++){
                FreeSubTree(current->childNodes[i]);
            }
        }
    }

    FreeNode(current);
}

void FreeNode(bTNode *node){
    if(node->type==IS_STRING){
        free(node->stringVal);
    }
    free(node->childNodes);
    free(node);
}

/**
 * Delete node on the given path if it exists
 * @param path
 * @param depth
 */
void BTreeDelete(bTree* bt ,char** path, int depth){
    bTNode *tmp;
    tmp = bt->root;

    if(depth==0){
        tmp = Find(tmp, path[0]);
    }
    else{
        tmp = FindPath(bt, path, depth);
    }

    if(tmp == NULL) {
        printf("node does not exist");
        return;
    }
    else{
        DeleteNode(tmp->parent, tmp->name);
    }

}

bTNode* FindPath(bTree* bt ,char **path, int depth){
    if(depth==0){
        return Find(bt->root, path[0]);
    }

    bTNode* tmp = bt->root;
    for(int i=0; i<depth; i++){
        tmp = Find(tmp, path[i]);
        if(tmp == NULL) {
            return NULL;
        }
    }

    return tmp;
}

void DeleteNode(bTNode *parent, char* key){
    // If parent folder becomes empty, delete it

    bTNode** childNodes = parent->childNodes;
    int s = parent->nrOfChildNodes;
    // find the node to delete, and fix the possible gap in the sorted array
    int i = 0;
    for(; i<s; i++){
        if(strcmp(childNodes[i], key)==0){
            bTNode* tmp = childNodes[i];
            childNodes[i] = NULL;
            if(tmp->type==IS_FOLDER) {
                FreeSubTree(tmp);
            }
            else{
                FreeNode(tmp);
            }
        }
    }

    // If the deletion resulted in a gap, move the section between (i+1) and s one step back
    if(i<(s-1)){
        memmove(&parent->childNodes[i], &parent->childNodes[i + 1], (s - i) * sizeof(bTNode *));
    }
    parent->nrOfChildNodes--;

    if(parent->nrOfChildNodes==0){
        DeleteNode(parent->parent, parent->name);
    }
}


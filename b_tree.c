/**
 * Dynamic tree structure. Binds a key to a value(int or string).
 * The tree has N number of nodes, the initial size will be of size 10. Will be decreased or increased dynamically.
 * Every level of the tree is kept sorted.
 * A node can be of type:
 * folder : Holds no value but a variable number of child nodes,
 * numeric : Holds an integer value,
 * string : Holds a string value.
 *
 * @author Joakim Söderstrand
 */

#include "b_tree.h"


void *MallocSafe(size_t size) {
    void *result;

    result = malloc(size);

    if (result != NULL) {
        return result;
    }

    printf("error: malloc failed\n");
    exit(-1);
}

/**
 * Setting the initial size of the child-array to 10
 * @return
 */
bTNode *CreateNewNode(bTree *bt, int valueType, char* name) {
    bTNode *newNode = (bTNode*) MallocSafe(sizeof(bTNode));

    if(valueType==IS_FOLDER) {
        newNode->childNodeCapacity = INITIAL_CHILD_CAPACITY;
        newNode->childNodes = (bTNode **) MallocSafe(newNode->childNodeCapacity * sizeof(bTNode *));
    }

    newNode->nrOfChildNodes = 0;

    newNode->name = (char*) MallocSafe(strlen(name)+1 * sizeof(char));
    strcpy(newNode->name, name);
    newNode->type = valueType;

    return newNode;
}


/**
 * Create a new bTree and return the pointer.
 * Will set root when initializing since it will be fixed without values(only child nodes).
 * @return
 */
bTree *CreateBTree() {
    bTree *tree = (bTree *) malloc(sizeof(bTree));
    tree->root = CreateNewNode(tree, IS_FOLDER, "root");
    tree->root->parent = NULL;

    return tree;
}

/**
 * Insert new node into parent-folder child array
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

    // The sorting mechanism
    if (Compare(new, current->childNodes[m]) > 0) {
        for (int i = m; i < s; i++) {

            // update key with new node
            if (Compare(new, current->childNodes[i]) == 0) {
                // Do nothing if folder
                if(new->type == IS_FOLDER){
                    FreeNode(new);
                    return;
                }
                    // Replace the value
                else{
                    ReplaceValue(current->childNodes[i], new);
                    FreeNode(new);
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

        // if this point is reached the new node key is the biggest and just gets appended
        new->parent = current;
        current->childNodes[s] = new;
        current->nrOfChildNodes++;
        return;
    }
    else {
        for (int i = 0; i < s; i++) {
            bTNode *child = current->childNodes[i];

            // if the node already exists
            if (strcmp(new->name, child->name) == 0) {
                // Do nothing if folder
                if (new->type == IS_FOLDER) {
                    FreeNode(new);
                    return;
                }
                    // Replace the value
                else {
                    ReplaceValue(current->childNodes[i], new);
                    FreeNode(new);
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
 * Insert a new node on the given path
 * @param bt
 * @param dots
 * @param path
 * @param type
 * @param name
 * @param val
 */
void BTreeInsert(bTree *bt, char **path, int type, char *name, void *val) {
    bTNode *new = CreateNewNode(bt, type, name);
    new->type = type;

    if (new->type == IS_STRING) {
        new->stringVal = (char*) malloc(strlen(val)+1 * sizeof(char));
        strcpy(new->stringVal, val);
    }
    else {
        new->value = (int) val;
    }

    // If there is no path, insert at root.
    if (path==NULL) {
        if (bt->root->nrOfChildNodes == 0) {
            bt->root->childNodes[0] = new;
            bt->root->nrOfChildNodes++;
            new->parent = bt->root;
            return;
        }

        Insert(bt->root, new);
    }
    else {
        // Check if the node exist, will replace value if it does.
        bTNode *tmp = FindWithPath(bt, path);

        if (tmp != NULL) {
            Insert(tmp->parent, new);
            return;
        }

        // First check if the sub-path exists, if not, create the folder missing.
        bTNode* parent = bt->root;
        while (strcmp(*path, name)!=0){
            tmp = Find(parent, *path);
            if(tmp==NULL){
                tmp = CreateNewNode(bt, IS_FOLDER, *path);
                Insert(parent, tmp);
            }
            parent = tmp;
            path++;
        }
        // insert the value in path (path[0].path[1]....path[N].key-value)
        Insert(parent, new);
    }
}

int Compare(bTNode *n1, bTNode *n2) {
    return strcmp(n1->name, n2->name);
}

/**
 * Recursively print the tree from current node to leaf-nodes.
 * @param node
 */
void PrintNode(bTNode *node) {
    printf("%s ", node->name);

    if(node->type==IS_STRING){
        printf(": %s\n", node->stringVal);
    }
    else if(node->type==IS_NUMERIC){
        printf(": %d\n", node->value);
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
}

/**
 * Print the whole tree
 * @param bt
 */
void PrintBTree(bTree *bt) {
    PrintNode(bt->root);
}


void ReplaceValue(bTNode *old, bTNode *new){
    // If the new value is of a different type print error message
    if(old->type != new->type){
        return;
    }


    if(new->type==IS_STRING){
        old->stringVal = realloc(old->stringVal, strlen(new->stringVal)+1);
        strcpy(old->stringVal, new->stringVal);
    }
    else{
        old->value = new->value;
    }
}

/**
 * Recursively search search a folder for the node with the given key
 * @param node
 * @param key
 * @return
 */
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

/**
 * This function requires strings to be placed in a folder called strings.. If the folder does not exist
 * NULL is returned.
 * If the key with the given language prefix does not exist, a recursive search will be started from /string.
 * @param bt
 * @param key
 * @param lan
 * @return
 */
char* GetText(bTree* bt, char *key, char *lan){
    if(Find(bt->root, "strings")==NULL){
        puts("could not locate 'strings'-folder..");
        return NULL;
    }

    char *path[4] = {"strings", lan, key, END_OF_PATH};
    bTNode* tmp = FindWithPath(bt, path);

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
char* GetString(bTree* bt, char** path){
    bTNode* tmp = FindWithPath(bt ,path);
    if(tmp==NULL){
        printf("node does not exist on this path..\n");
        return NULL;
    }   else if(tmp->type != IS_STRING){
        printf("node does not hold a string value.. \n");
        return NULL;
    }

    else {
        return tmp->stringVal;
    }
}

int GetInt(bTree *bt, char** path){
    bTNode* tmp = FindWithPath(bt ,path);
    if(tmp==NULL){
        printf("node does not exist on this path..\n");
        return NULL;
    }
    else if(tmp->type != IS_NUMERIC){
        printf("node does not hold a numeric value.. \n");
        return NULL;
    }

    else {
        return tmp->value;
    }
}

/**
 * Print the nodes in the current folder that holds a value.(skip folders)
 * @param bt
 * @param path
 * @param depth
 */
void Enumerate(bTree *bt, char** path){
    bTNode *tmp = FindWithPath(bt, path);

    if(tmp == NULL){
        printf("node does not exist.\n");
        return;
    } else if(tmp->type != IS_FOLDER){
        if(tmp->type == IS_STRING){
            printf("%s : %s\n", tmp->name, tmp->stringVal);
        } else{
            printf("%s : %d\n", tmp->name, tmp->value);
        }

        return;
    }

    bTNode** children = tmp->childNodes;
    for(int i=0; i<tmp->nrOfChildNodes; i++) {
        if(children[i]->type == IS_STRING){
            printf("%s : %s\n", children[i]->name, children[i]->stringVal);
        } else if(children[i]->type==IS_NUMERIC){
            printf("%s : %d\n", children[i]->name, children[i]->value);
        }
    }

}

/**
 * Return the type of the node or -1 if it does not exist.
 * @param bt
 * @param path
 * @return
 */
int GetType(bTree *bt, char** path){
    bTNode* node = FindWithPath(bt, path);
    if(node == NULL){
        return -1;
    }
    return node->type;
}

/**
 * Generic function that returns the value of a given node. Need to specify type!
 * Null is return if error.
 * @param bt
 * @param path
 * @param type
 * @return
 */
void* GetValue(bTree *bt, char** path, int type){
    bTNode* tmp = FindWithPath(bt, path);

    if(tmp==NULL){
        return NULL;
    }

    if(type!=tmp->type||tmp->type==IS_FOLDER){
        return NULL;
    }

    if(tmp->type == IS_NUMERIC){
        return tmp->value;
    } else {
        return tmp->stringVal;
    }
}

/**
 * Generic method to set the value of a node on the given path.
 * @param bt
 * @param path
 * @param type
 * @param value
 */
void SetValue(bTree *bt, char** path, int type, void* value){
    bTNode* tmp = FindWithPath(bt, path);
    if(tmp == NULL){
        printf("node does not exist\n");
        return;
    }

    if(tmp->type != type) {
        printf("node is of wrong type\n");
        return;
    }

    if(type==IS_STRING) {
        char* new = (char*)value;
        tmp->stringVal = realloc(tmp->stringVal, strlen(new)+1);
        strcpy(tmp->stringVal, new);
        puts(tmp->stringVal);
        return;
    }

    tmp->value = value;
}


/**
 * Search for the given node, from the given folder.
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

/**
 * Find with path
 * @param bt
 * @param path
 * @return
 */
bTNode* FindWithPath(bTree* bt, char **path){
    bTNode* tmp = bt->root;

    if(strcmp(path[1], END_OF_PATH)==0&&strcmp(path[0], "root")==0){
        return tmp;
    }

    while(strcmp(*path, END_OF_PATH)!=0){
        tmp = Find(tmp, *path);
        if(tmp == NULL){
            return NULL;
        }
        path++;
    }
    return tmp;
}

/**
 * Delete node on the given path if it exists
 * If the node to delete is at root level use ["nameOfNode", "END_OF_PATH"]
 * @param path
 * @param depth
 */
void BTreeDelete(bTree* bt ,char** path){
    bTNode *tmp;

    // If path[1] == END_OF_PATH path[0] = name of node
    if(strcmp(path[1], END_OF_PATH)==0){
        tmp = Find(bt->root, path[0]);
    }
    else{
        tmp = FindWithPath(bt, path);
    }

    if(tmp == NULL) {
        printf("node does not exist\n");
        return;
    }
    else{
        DeleteNode(tmp->parent, tmp->name);
    }

}

/**
 * Recursive method, delete the given node from its parent, if parent(folder) becomes empty delete it.
 * @param parent
 * @param key
 */
void DeleteNode(bTNode *parent, char* key){
    bTNode** childNodes = parent->childNodes;
    int s = parent->nrOfChildNodes;
    // find the node to delete, and fix the possible gap in the sorted array
    int i = 0;
    for(; i<s; i++){
        if(strcmp(childNodes[i]->name, key)==0){
            bTNode* tmp = childNodes[i];
            childNodes[i] = NULL;
            if(tmp->type==IS_FOLDER) {
                FreeSubTree(tmp);
            }
            else{
                FreeNode(tmp);
            }
            parent->nrOfChildNodes--;
            break;
        }
    }

    // If the deletion resulted in a gap, move the section between (i+1) and s one step back
    if(i<(s-1)){
        memmove(&parent->childNodes[i], &parent->childNodes[i + 1], (s - i) * sizeof(bTNode *));
    }

    // shrink the size of the array if N<SIZE/2, capacity>10
    if(parent->childNodeCapacity > 10 && (s*2) < parent->childNodeCapacity){
        parent->childNodes = realloc(parent->childNodes, (parent->childNodeCapacity/2) * sizeof(bTNode*));
        parent->childNodeCapacity = parent->childNodeCapacity/2;
        if(parent->childNodes == NULL){
            exit(EXIT_FAILURE);
        }
    }

    if(parent->nrOfChildNodes==0){
        // Don't delete the root node..
        if(parent->parent != NULL) {
            DeleteNode(parent->parent, parent->name);
        }
    }
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
    if(node->type == IS_FOLDER){
        free(node->childNodes);
    }


    free(node->name);
    free(node);
}

void FreeBTree(bTree *bt){
    FreeSubTree(bt->root);
    free(bt);
}




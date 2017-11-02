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

    if(bt->root->nrOfChildNodes == 0){
        bt->root->childNodes[0] = new;
        bt->root->nrOfChildNodes++;
        return;
    }

    Insert(bt->root  ,new);
}

int Compare(bTNode* n1, bTNode* n2){
    return strcmp(n1->name, n2->name);
}


void PrintNode(bTNode* node){
    bTNode* tmp;

    if(tmp->childNodes == 0){
        return;
    }

    for(int i=0; i<node->nrOfChildNodes; i++){
        tmp = node->childNodes[i];
        printf("%s, ", tmp->name);
    }

    printf("\n");
}

void PrintBTree(bTree* bt){
    PrintNode(bt->root);
}


/**
 * Insert into array and maintain a sorted order.
 * @param new
 */
void Insert(bTNode* current, bTNode* new){
    int s = current->nrOfChildNodes;
    int m = current->nrOfChildNodes/2;


    if(Compare(new, current->childNodes[m]) > 0){
        for(int i=m; i<s; i++){

            // update key with new node
            if(Compare(new, current->childNodes[i]) == 0){
                return;
            }

            if (Compare(new, current->childNodes[i]) < 0){
                memmove(&current->childNodes[i+1], &current->childNodes[i], (s-i)*sizeof(bTNode*));
                current->nrOfChildNodes++;
                return;
            }
        }

        // if this point is reached the new nodes key is the biggest and just gets appended
        current->childNodes[s] = new;
        current->nrOfChildNodes++;
        return;
    } else {
        for(int i=0; i<s; i++){
            if(Compare(new, current->childNodes[i])== 0){
                return;
            }

            if (Compare(new, current->childNodes[i]) < 0){
                memmove(&current->childNodes[i+1], &current->childNodes[i], (s-i)*sizeof(bTNode*));
                current->childNodes[i] = new;
                current->nrOfChildNodes++;
                return;
            }
        }
    }
}

/**
 * Search for the given node.
 * @return
 */
bTree* Find(bTNode* node, char* key){
    if(node->nrOfChildNodes==0){
        printf("%s, is not in this tree", key);
        return NULL;
    }
}

bTNode* delete(bTNode *root, int data);




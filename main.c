#include <stdio.h>
#include "b_tree_test.h"
#include "parser.h"

/**
 * Iterate over child nodes, return true if they are sorted, false if not.
 * @param node
 * @return
 */
bool Sorted(bTNode *node){
    int s = node->nrOfChildNodes;
    bTNode **nodes = node->childNodes;
    for(int i=0; i<s-1; i++){
        if(strcmp(nodes[i]->name, nodes[i+1]->name)>0){
            return false;
        }
    }
    return true;
}

/**
 * Recursively check if current and its child nodes are sorted
 */
void AssertSorted(bTNode *current){
    if(current->type == IS_FOLDER&&current->nrOfChildNodes!=0){
        assert(Sorted(current));
    }
    else {
        return;
    }

    int s=current->nrOfChildNodes;
    bTNode** nodes = current->childNodes;

    for(int i=0; i<s; i++){
        if(nodes[i]->type==IS_FOLDER){
            AssertSorted(nodes[i]);
        }
    }
}

/**
 * Print all k-v at a given level(folder/node) (not printing child folders)
 * @param bt
 */
void EnumerateLevel(bTree *bt){
    // root does not contain any key-values only folders thus should not print anything
    printf("Enumerate path : root \n");
    char* path[2] = {"root", END_OF_PATH};
    Enumerate(bt, path);
    puts("");

    // strings.no should contain 3 key-values
    printf("Enumerate path : root.strings.no\n");
    char* path2[3] = {"strings", "no", END_OF_PATH};
    Enumerate(bt, path2);
}


void IsSorted(bTree *bt){
    // Check that all levels in tree is sorted
    bTNode *current = bt->root;
    AssertSorted(current);
}

void DeleteSomeNodes(bTree *bt){
    char* path[4] = {"strings", "no", "text", END_OF_PATH};
    BTreeDelete(bt, path);
    assert(FindWithPath(bt, path) == NULL);

    // strings.se only contains one k-v, deleting should result in the whole folder to be deleted.
    path[1] = "se"; path[2] = "header";
    BTreeDelete(bt, path);
    assert(FindWithPath(bt, path) == NULL);
}

void GetSomeText(bTree *bt){
    // Should contain 'Oppdater programvaren'
    char *asserted = "Oppdater programvaren";
    char *actual = GetText(bt, "text", "no");

    char *path[4] = {"strings", "se", "button_cancel", END_OF_PATH};
    // 'strings.se'-folder does not contain the key 'button_cancel'.
    assert(FindWithPath(bt, path)==NULL);

    // When using GetText() however, a recursive search in strings should find an alternative if ofc it
    // exists (as in this case). .
    char *alternative = GetText(bt, "button_cancel", "se");
    assert(alternative!=NULL);
    printf("alternative value: %s\n", alternative);
}

int main() {
    // Allocate buffer for reading the key-value pairs
    char** buffer = (char **) malloc(100 * sizeof(char*));
    keyValue** kvBuffer = (keyValue **) malloc(100 * sizeof(keyValue*));

    for(int i=0; i<100; i++){
        buffer[i] = malloc(250 * sizeof(char));
    }


    int lines = ReadFile(buffer);

    for (int i=0; i<lines; i++){
        puts(buffer[i]);
    }

    //RunBtreeTests();

    for(int i=0; i<100; i++){
        free(buffer[i]);
    }

    free(buffer);

    printf("-----Test Enumerate()-----\n \n");

    //EnumerateLevel(bt);

    printf("\n \n -----Test GetText()-----\n \n");

    //GetSomeText(bt);


    //IsSorted(bt);
    //DeleteSomeNodes(bt);
    //PrintBTree(bt);

    //FreeBTree(bt);

}


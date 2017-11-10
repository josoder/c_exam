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


void GetValueType(bTree *bt) {
    char* path[4] = {"strings", "no", "text", END_OF_PATH};
    assert(GetType(bt, path)==IS_STRING);
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

void SetGetValue(bTree *bt){
    char* path[4] = {"strings", "en", "button_cancel", END_OF_PATH};
    char *val = GetValue(bt, path, IS_STRING);
    assert(strcmp(val, "Cancel")==0);
    // should return NULL
   // assert(GetValue(bt, path, IS_NUMERIC)==NULL);

    //SetValue(bt, path, IS_STRING, "cancel2");
    //assert(strcmp(GetValue(bt, path, IS_STRING), "cancel2")==0);

    SetValue(bt, path, IS_STRING, "Cancel");
    // should not change the value
    SetValue(bt, path, IS_NUMERIC, 234);
}

void GetSomeText(bTree *bt){
    // Should contain 'Oppdater programvaren'
    char *asserted = "Oppdater programvaren";
    char *actual = GetText(bt, "text", "no");

    char *path[4] = {"strings", "se", "button_cancel", END_OF_PATH};
    // 'strings.se'-folder does not contain the key 'button_cancel'.
    assert(FindWithPath(bt, path)==NULL);

    puts("Trying to get the text value from 'button_cancel' with lang = 'se' "
                 "(It does not exist but an alternative lang does)");
    // When using GetText() however, a recursive search in strings should find an alternative if ofc it
    // exists (as in this case). .
    char *alternative = GetText(bt, "button_cancel", "se");
    assert(alternative!=NULL);
    printf("alternative value: %s\n", alternative);
}

void InsertLine(bTree *bt ,char **path, int type, void* val){
    char *name;

    //Get the name from the path
    int i = 0;
    while(strcmp(path[i], END_OF_PATH)!=0){
        name = path[i++];
    }

    if(type==IS_STRING){
        BTreeInsert(bt, path, IS_STRING, name, val);
    }

    else{
        BTreeInsert(bt, path, IS_NUMERIC, name, val);
    }
}

// Set up the tree with key-value pairs parsed from keyvalue.txt
int main() {
    // Allocate buffer for reading the key-value pairs
    char** buffer = (char **) malloc(100 * sizeof(char*));
    for(int i=0; i<100; i++){
        buffer[i] = malloc(250 * sizeof(char));
    }

    bTree *bt = CreateBTree();

    // Read lines from file
    int lines = ReadFile(buffer);

    char* kB;
    char key[100]; char *k;
    char value[100]; char *v;
    char* path[10];

    // Separate keys from values and insert into the tree
    for (int i=0; i<lines; i++){
        kB = strdup(buffer[i]);
        k = key;
        v = value;
        k = strsep(&buffer[i], "=");
        v = strsep(&buffer[i], "=");
        if (k == NULL || v == NULL) {
            printf("Wrong file format\n");
            exit(EXIT_FAILURE);
        } else {
            Trim(k);
            Trim(v);
            ExtractPath(k, path);

            if(*v!='"'){
                char *end = NULL;
                long val = strtol(v, &end, 10);
                if(end == v){
                    printf("wrong format");
                    exit(EXIT_FAILURE);
                }
                InsertLine(bt, path, IS_NUMERIC, (int)val);
            } else {
                // remove leading and trailing '"' from strings, annoying to handle(needs escaping).
                ReformatValue(v);
                Trim(v);
                InsertLine(bt, path, IS_STRING, v);
            }

            free(kB);
            free(k);
        }
    }

    // Free buffer

    for(int i=0; i<100; i++){
        free(buffer[i]);
    }

    free(buffer);



    /**
     * Run some tests
     */

    printf("-----Test Enumerate()-----\n \n");

    EnumerateLevel(bt);

    printf("\n \n -----Test GetText()-----\n \n");

    GetSomeText(bt);

    printf("\n \n -----Test Set- and GetValue()-----\n \n");

    SetGetValue(bt);


    GetValueType(bt);
    IsSorted(bt);
    DeleteSomeNodes(bt);
    PrintBTree(bt);

    FreeBTree(bt);

}


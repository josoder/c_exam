#include <stdio.h>
#include "b_tree_test.h"
#include "parser.h"


static const char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";


void EnumerateLevel(bTree *bt){
    char* path[3] = {"config", "update", END_OF_PATH};
    Enumerate(bt, path);
}

int main() {
    //RunBtreeTests();
    bTree *bt;
    bt = CreateBTree();
    PopulateBTree(bt);
    //PrintBTree(bt);
    EnumerateLevel(bt);
}


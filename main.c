#include <stdio.h>
#include <assert.h>
#include "b_tree.h"


int main() {
    bTree* tree = CreateBTree();
    assert(tree!=NULL);
    BTreeInsert(tree, IS_FOLDER, "test", NULL);
    assert(tree!=NULL);
}
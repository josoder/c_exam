#include <stdio.h>
#include <assert.h>
#include "b_tree.h"


int main() {
    bTree* tree = CreateBTree();
    assert(tree!=NULL);
    BTreeInsert(tree, NO_VALUE, "test", NULL);

}
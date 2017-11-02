#include <stdio.h>
#include <assert.h>
#include "b_tree.h"
#include "bubble_sort_string.h"

void TestBubble(){
    char* array[5] = {"z", "g", "f", "i", "a"};
    Sort(array, 5);
    for(int i=0; i<5; i++){
        puts(array[i]);
    }
}


int main() {
    TestBubble();
    bTree* tree = CreateBTree();
    assert(tree!=NULL);
    BTreeInsert(tree, IS_FOLDER, "test", NULL);
    assert(tree!=NULL);
}
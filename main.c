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

void TestInsert(bTree* bt){
    BTreeInsert(bt, IS_FOLDER, "c", NULL);
    BTreeInsert(bt, IS_FOLDER, "a", NULL);
    BTreeInsert(bt, IS_FOLDER, "b", NULL);
}

void StringSplit() {
    char str[] = "This is a sample string, just testing.";
    char *p;

    printf("Split \"%s\" in tokens:\n", str);

    p = strtok(str, " ");

    while (p != NULL) {
        printf("%s\n", p);
        p = strtok(NULL, " ,");
    }
}


int main() {
    TestBubble();
    bTree* tree = CreateBTree();
    assert(tree!=NULL);
    TestInsert(tree);
    PrintBTree(tree);
}


#include <stdio.h>
#include <assert.h>
#include "b_tree.h"

static const char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void TestInsert(bTree* bt){
    BTreeInsert(bt, 0, NULL, IS_FOLDER, "c", NULL);
    BTreeInsert(bt, 0, NULL, IS_FOLDER, "a", NULL);
    BTreeInsert(bt, 0, NULL, IS_FOLDER, "b", NULL);

    char* path[3] = {"first", "second", "third"};
    BTreeInsert(bt, 3, path, IS_FOLDER, "k", NULL);
    BTreeInsert(bt, 3, path, IS_STRING, "string", "hej!");
}

void TestDelete(bTree* bt){
    TestInsert(bt);

    // TestInsert will insert one key-value pair, [k:string, v:"hej!"] in the folder "third",
    // with the path root.first.second.third.string="hej!".
    // This means all folders on the given path will be empty after deleting the third one thus
    // should result in that the whole path gets deleted.

    char* path[3] = {"first", "second", "third"};
    assert(FindPath(bt, path, 3, path[2])!=NULL);

    BTreeDelete(bt, path, 3);
    assert(FindPath(bt, path, 3, "third")==NULL);
    assert(FindPath(bt, path, 2, "second") == NULL);
    assert(FindPath(bt, NULL, 0, "first")==NULL);
}

// Capacity should double when the initial capacity(10) is reached.
void TestSizeIncreasesWhenCapacityIsReached(bTree* bt){
    char tmp[5];
    for(int i=0; i<11; i++){
        sprintf(tmp, "%d", i);
        BTreeInsert(bt, 0, NULL, IS_FOLDER, tmp, NULL);
    }
    assert(bt->root->childNodeCapacity == 20);
}

void TestInsertAndReplaceString(bTree* bt){
    char* testKey = "key";

    BTreeInsert(bt, 0, NULL, IS_STRING, testKey, "testing");

    char* value = (char*) Find(bt->root, testKey)->stringVal;

    assert(strcmp(value, "testing")==0);

    char* val = "123";

    BTreeInsert(bt, 0, NULL, IS_STRING, testKey, val);

    char* fromNode = Find(bt->root, testKey)->stringVal;

    assert(strcmp(fromNode, "123")==0);
}

void TestReplaceSwitchType(bTree* bt){
    char* testKey = "key";

    BTreeInsert(bt, 0, NULL, IS_STRING, testKey, "testing");

    char* value = (char*) Find(bt->root, testKey)->stringVal;

    assert(strcmp(value, "testing")==0);

    int val = 123;

    BTreeInsert(bt, 0, NULL, IS_NUMERIC, testKey, val);

    int fromNode = Find(bt->root, testKey)->value;

    assert(fromNode == 123);
    assert(Find(bt->root, testKey)->type==IS_NUMERIC);
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

void TestFindPath(bTree* bt) {
    TestInsert(bt);
    char* path[3] = {"first", "second", "third"};
    char* name = (FindPath(bt, NULL, 0, "first"))->name;
    assert(strcmp(name, "first")==0);
    name = (FindPath(bt, path, 3, "third"))->name;
    printf(name);
}

void TestFreeBTree(bTree* bt){
    FreeBTree(bt);
}

void TestGetText(bTree* bt){
    BTreeInsert(bt, 0, NULL, IS_FOLDER, "strings", NULL);

    char* path[2] = {"strings", "en"};
    BTreeInsert(bt, 2, path, IS_STRING, "greeting", "hello");
    path[1] = "sv";
    BTreeInsert(bt, 2, path, IS_STRING, "greeting", "hej");
    char* swedishGreeting = GetText(bt, "greeting", "sv");
    assert(strcmp(swedishGreeting, "hej")==0);
    // Does not exist, but should return a greeting in different language
    char* germanGreeting = GetText(bt, "greeting", "de");
    assert(germanGreeting!=NULL);
}



int main() {
    bTree* tree = CreateBTree();
    assert(tree!=NULL);
    //TestInsert(tree);
    //Find(tree->root, "q");
    //printf("\n");
    //PrintBTree(tree);
    //TestSizeIncreasesWhenCapacityIsReached(tree);
    //TestInsertAndReplaceString(tree);
    //TestReplaceSwitchType(tree);
    //TestFreeBTree(tree);
    //TestDelete(tree);
    TestGetText(tree);
    PrintBTree(tree);
    //TestFindPath(tree);
}


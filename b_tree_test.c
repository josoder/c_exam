//
// Created by josoder on 06.11.17.
//

#include "b_tree_test.h"

void RunBtreeTests(){
    bTree* tree = CreateBTree();
    assert(tree!=NULL);
    TestSizeIncreasesWhenCapacityIsReached(tree);
    TestInsertAndReplaceString(tree);
    TestReplaceWrongType(tree);
    TestGetText(tree);
    TestGetType(tree);
    TestGetString(tree);
    TestGetInt(tree);
    TestGetValue(tree);
    TestSetValue(tree);
    TestEnumerate(tree);
    TestDelete(tree);

    PrintBTree(tree);

    FreeBTree(tree);
}

void TestDelete(bTree* bt){
    // TestInsert will insert one key-value pair, [k:string, v:"hej!"] in the folder "third",
    // with the path root.first.second.third.string="hej!".
    // This means all folders on the given path will be empty after deleting the third one thus
    // should result in that the whole path gets deleted.

    PrintBTree(bt);
    char* path[2] = {"strings", "en"};


    BTreeDelete(bt, path, 2);

    assert(FindPath(bt, path, 2) == NULL);
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

void TestReplaceWrongType(bTree* bt){
    char* testKey = "key";

    BTreeInsert(bt, 0, NULL, IS_STRING, testKey, "testing");

    char* value = (char*) Find(bt->root, testKey)->stringVal;

    assert(strcmp(value, "testing")==0);

    int val = 123;

    BTreeInsert(bt, 0, NULL, IS_NUMERIC, testKey, val);

    // Should still be "testing
    assert(strcmp(Find(bt->root, testKey)->stringVal, value)==0);

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

void TestGetType(bTree* bt){
    char **path[3] = {"strings", "en", "greeting"};
    assert(GetType(bt, path,3)==IS_STRING);
    path[2] = "hey!";
    // Should return -1 when the key does not exist.
    assert(GetType(bt, path, 3));
}

void TestGetString(bTree* bt){
    char *path[3] = {"strings", "en", "greeting"};
    assert(strcmp(GetString(bt, path, 3), "hello")==0);

    assert(GetString(bt, path, 2)==NULL);
}

void TestGetInt(bTree* bt){
    char *path[3] = {"config", "updates", "rate"};
    BTreeInsert(bt, 2, path, IS_NUMERIC, "rate", 12);
    PrintBTree(bt);
    assert(GetInt(bt, path, 3)==12);
}

void TestGetValue(bTree* bt){
    char *path[3] = {"strings", "en", "greeting"};
    assert(strcmp(GetValue(bt, path, 3), "hello")==0);
    path[0] = "config"; path[1] = "updates"; path[2] = "rate";
    assert(GetValue(bt, path, 3)==12);
}

void TestSetValue(bTree* bt){
    char *path[3] = {"strings", "en", "greeting"};
    SetValue(bt, path, 3, IS_STRING, "hey");
    assert(strcmp(GetValue(bt, path, 3), "hey")==NULL);
    path[0] = "config"; path[1] = "updates"; path[2] = "rate";
    SetValue(bt, path, 3, IS_NUMERIC, 13);
    // Should print error message. Value should still be 13.
    SetValue(bt, path, 3, IS_STRING, "thirteen");
    assert(GetValue(bt, path, 3)==13);
}

void TestEnumerate(bTree* bt){
    char *path[3] = {"strings", "en", "what"};
    BTreeInsert(bt, 2, path, IS_STRING, "what", "ever");
    Enumerate(bt, path, 2);
}
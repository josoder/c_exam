//
// Created by josoder on 06.11.17.
//

#include "b_tree_test.h"

void RunBtreeTests(){
    bTree* tree = CreateBTree();
    assert(tree!=NULL);

    //PrintBTree(tree);
    TestInsertAndReplaceString(tree);
    //TestReplaceWrongType(tree);
    //TestGetText(tree);
    //TestGetInt(tree);
    //TestGetValue(tree);
    //TestSetValue(tree);
    //TestEnumerate(tree);
    //TestGetString(tree);
    //TestGetType(tree);
    //TestDelete(tree);
    //TestSizeIncreasesWhenCapacityIsReached(tree);

    // Should print an empty tree
    printf("Tree: \n");
    PrintBTree(tree);

    FreeBTree(tree);
}


void TestDelete(bTree* bt){
    char* path[2] = {"strings", END_OF_PATH};


    BTreeDelete(bt, path);
    path[0] = "config";
    BTreeDelete(bt, path);

    assert(FindWithPath(bt, path) == NULL);
}

// Capacity should double when the initial capacity C(10) is reached and shrink when N*2<C and C > 10
void TestSizeIncreasesWhenCapacityIsReached(bTree* bt){
    char tmp[5];
    for(int i=0; i<11; i++){
        sprintf(tmp, "%d", i);
        BTreeInsert(bt, NULL, IS_FOLDER, tmp, NULL);
    }

    assert(bt->root->childNodeCapacity == 20);


    // Should shrink when deleting the nodes again.

    char* path[2] = {"", END_OF_PATH};

    for(int i=0; i<11; i++){
        sprintf(tmp, "%d", i);
        path[0] = tmp;
        BTreeDelete(bt, path);
    }

    assert(bt->root->childNodeCapacity == 10);
}

void TestInsertAndReplaceString(bTree* bt){
    char* testKey = "key";
    char* path[4] = {"en", "something", testKey, END_OF_PATH};

    BTreeInsert(bt, path, IS_NUMERIC, testKey, 2);
    path[1] = "something else";
    BTreeInsert(bt, path, IS_NUMERIC, testKey, 2);
    char* val = "123";

    BTreeInsert(bt, NULL, IS_STRING, testKey, val);

    char* fromNode = Find(bt->root, testKey)->stringVal;

    assert(strcmp(fromNode, "123")==0);

    //char* path[2] = {"key", END_OF_PATH};

    //BTreeDelete(bt, path);
}

void TestReplaceWrongType(bTree* bt){
    char* testKey = "key";

    BTreeInsert(bt, NULL, IS_STRING, testKey, "testing");

    char* value = Find(bt->root, testKey)->stringVal;

    assert(strcmp(value, "testing")==0);

    int val = 123;

    BTreeInsert(bt, NULL, IS_NUMERIC, testKey, val);

    // Should still be "testing
    assert(strcmp(Find(bt->root, testKey)->stringVal, value)==0);

    char* path[2] = {"key", END_OF_PATH};
    BTreeDelete(bt, path);

}


void TestGetText(bTree* bt){
    BTreeInsert(bt, NULL, IS_FOLDER, "strings", NULL);

    char* path[3] = {"strings", "en", "greeting", END_OF_PATH};
    BTreeInsert(bt, path, IS_STRING, "greeting", "hello");
    path[1] = "sv";
    BTreeInsert(bt, path, IS_STRING, "greeting", "hej");
    char* swedishGreeting = GetText(bt, "greeting", "sv");
    assert(strcmp(swedishGreeting, "hej")==0);
    // Does not exist, but should return a greeting in different language
    char* germanGreeting = GetText(bt, "greeting", "de");
    assert(germanGreeting!=NULL);
}

void TestGetType(bTree* bt){
    char *path[4] = {"strings", "en", "greeting", END_OF_PATH};
    assert(GetType(bt, path)==IS_STRING);
    path[2] = "hey!";
}

void TestGetString(bTree* bt){
    char *path[4] = {"strings", "en", "greeting", END_OF_PATH};
    assert(strcmp(GetString(bt, path), "hey")==0);
}

void TestGetInt(bTree* bt){
    char *path[4] = {"config", "update", "rate", END_OF_PATH};
    BTreeInsert(bt, path, IS_NUMERIC, "rate", 12);
    assert(GetInt(bt, path)==12);
}

void TestGetValue(bTree* bt){
    char *path[4] = {"strings", "en", "greeting", END_OF_PATH};
    assert(strcmp(GetValue(bt, path), "hello")==0);
}

void TestSetValue(bTree* bt){
    char *path[4] = {"strings", "en", "greeting", END_OF_PATH};
    SetValue(bt, path, IS_STRING, "hey");
    assert(strcmp(GetValue(bt, path), "hey")==NULL);
    path[0] = "config"; path[1] = "update"; path[2] = "rate";
    SetValue(bt, path, IS_NUMERIC, 13);
    // Should print error message. Value should still be 13.
    SetValue(bt, path, IS_STRING, "thirteen");
    assert(GetValue(bt, path)==13);
}

void TestEnumerate(bTree* bt){
    char *path[3] = {"strings", "en", END_OF_PATH};
    BTreeInsert(bt, path, IS_STRING, "what", "ever");
    Enumerate(bt, path);
}
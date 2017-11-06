//
// Created by josoder on 06.11.17.
//

#ifndef EXAM_B_TREE_TEST_H
#define EXAM_B_TREE_TEST_H

#include "b_tree.h"
#include <assert.h>

void RunBtreeTests();

void TestDelete(bTree* bt);

void TestSizeIncreasesWhenCapacityIsReached(bTree* bt);

void TestInsertAndReplaceString(bTree* bt);

void TestReplaceWrongType(bTree* bt);

void TestGetText(bTree* bt);

void TestGetType(bTree* bt);

void TestGetString(bTree* bt);

void TestGetInt(bTree* bt);

void TestGetValue(bTree* bt);

void TestSetValue(bTree* bt);

void TestEnumerate(bTree* bt);

#endif //EXAM_B_TREE_TEST_H

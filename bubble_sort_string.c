//
// Created by josoder on 31.10.17.
//

#include "bubble_sort_string.h"

void Sort(char **array, int size){
    bool swapped = true;  // needed to get the sorting started
    while(swapped==true){
        swapped = false;
        for(int j=0; j<size-1; j++){
            if(strcmp(array[j], array[j+1])>0){
                swapped = true;
                Swap(&array[j], &array[j+1]);
            }
        }

        if(swapped == false)
            break;
    }
}


void Swap(char **a, char **b) {
    char *tmp = *a;
    *a = *b;
    *b= tmp;
}
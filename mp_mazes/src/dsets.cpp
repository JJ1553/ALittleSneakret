/* Your code here! */
#include "dsets.h"



void DisjointSets::addelements(int num) {
    for(int i = 0 ; i < num ; i++) {
        elements.push_back(-1);
    }
}

int DisjointSets::find(int elem) {
    if (elements[elem] < 0) {
        return elem;
    } else {
        
        elements[elem] = find(elements[elem]);
        return elements[elem];
    }
}


void DisjointSets::setunion(int a, int b){
    int tmpA = find(a);
    int tmpB = find(b);
    int newSize = elements[tmpA] + elements[tmpB];

    if ( elements[tmpA] > elements[tmpB] ) {
        elements[tmpA] = newSize;
        elements[tmpB] = tmpA;
    } 
    else {
        elements[tmpA] = tmpB;
        elements[tmpB] = newSize;
    }
}

int DisjointSets::size(int elem){
    int root = find(elem);  
    return elements[root] * (-1) ;
}
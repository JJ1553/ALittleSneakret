/**
 * @file bloom.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

 #include "bloom.h"

BF::BF(uint64_t size, std::vector<hashFunction> hashList){

    // Your code here 
    bv.resize(size, false);
    hv = hashList;
}

BF::BF(const BF& other){
    // Your code here 
    bv = other.bv;
    hv = other.hv;
}

BF::~BF(){
    return;
}

void BF::add(const int & key){

    // Your code here 
    for(const auto & hashFunction : hv) {
        int idx = hashFunction(key) % bv.size();
        bv[idx] = true;
    }
}

bool BF::contains(const int& key) const{
    // Your code here 

    for(const auto & hashFunction : hv) {
        int idx = hashFunction(key) % bv.size();
        if(!bv[idx])
        return false;
    }
    return true;

}

void BF::bit_union(const BF& other){
    // Your code here 
    for(unsigned i =0 ; i < bv.size() ; i++) {
        if(bv[i] || other.bv[i])
            bv[i] = true;
    }

}

void BF::intersect(const BF& other){
    // Your code here 
    for(unsigned i = 0 ; i< bv.size() ; i++) {
        if(!(bv[i] && other.bv[i]))
            bv[i] = false;
    }
}

float measureFPR(std::vector<int> inList, uint64_t size, std::vector<hashFunction> hashList, unsigned max){
    // Your code here

    BF bf(size, hashList);
    float tn = 0.0, fp = 0.0;
    for (unsigned i = 0; i < inList.size(); ++i) {
        bf.add(inList[i]);
    }
    for (unsigned i = 0; i < max ; i++) {
        bool guess = bf.contains(i);
        bool actual = false;
        for(unsigned j = 0; j < inList.size(); ++j) {
            if ((unsigned)inList[j] == i) 
                actual = true;
            }
        if (guess && !actual) 
        fp += 1.0; 
        else if (!guess && !actual) 
        tn += 1.0;
    }
    return fp / (tn + fp);

}

bool getBitFromArray(std::vector<char> bv, int index){
    // Your code here

    char c = bv[index / 8];
    return(getBitFromByte(c, index % 8));
}

bool getBitFromByte(char in, int index){
    // Your code here

    in >>= (7-index);
    return(in & 1) ==1;

}


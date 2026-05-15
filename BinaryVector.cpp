#include "BinaryVector.h"
#include <iostream>

BinaryVector::BinaryVector(const std::vector<Bit>& b) : bits(b) {}

BinaryVector::BinaryVector(int size) { 
    bits.resize(size, Bit(0)); 
}

BinaryVector::BinaryVector(std::initializer_list<int> list) {
    for (int val : list) {
        bits.push_back(Bit(val != 0));
    }
}

int BinaryVector::size() const { 
    return bits.size(); 
}

Bit BinaryVector::getBit(int index) const { 
    return bits[index]; 
}

void BinaryVector::append(const BinaryVector& other) {
    for (int i = 0; i < other.size(); ++i) {
        bits.push_back(other.getBit(i));
    }
}

void BinaryVector::print() const {
    for(Bit b : bits) std::cout << b.getVal();
    std::cout << "\n";
}
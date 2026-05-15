#pragma once
#include <vector>
#include <initializer_list>
#include "Bit.h"
#include <algorithm>

class BinaryVector {
protected:
    std::vector<Bit> bits;
public:
    BinaryVector(const std::vector<Bit>& b);
    BinaryVector(int size);
    BinaryVector(std::initializer_list<int> list);
    
    int size() const;
    Bit getBit(int index) const;
    
    void append(const BinaryVector& other);

    void reverse() {
        std::reverse(bits.begin(), bits.end());
    }
    
    void print() const;
};
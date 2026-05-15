#pragma once
#include <vector>
#include <initializer_list>
#include "Bit.h"

class BinaryVector {
protected:
    std::vector<Bit> bits;
public:
    // Các constructor
    BinaryVector(const std::vector<Bit>& b);
    BinaryVector(int size);
    BinaryVector(std::initializer_list<int> list); // Hỗ trợ khởi tạo nhanh: {1, 0, 1}
    
    int size() const;
    Bit getBit(int index) const;
    
    // Nối thêm 1 vector khác (dùng để tạo từ mã hệ thống)
    void append(const BinaryVector& other);
    
    void print() const;
};
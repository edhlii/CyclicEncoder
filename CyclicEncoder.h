#pragma once
#include "BinaryVector.h"

class CyclicEncoder {
private:
    BinaryVector g;
    int n, k, r;
public:
    CyclicEncoder(int n_val, int k_val, BinaryVector generator);
    BinaryVector encode(const BinaryVector& message);
};
#include "CyclicEncoder.h"
#include <iostream>
#include <iomanip>

CyclicEncoder::CyclicEncoder(int n_val, int k_val, BinaryVector generator) 
    : n(n_val), k(k_val), g(generator) {
    r = n - k;
}

BinaryVector CyclicEncoder::encode(const BinaryVector& message) {
    std::vector<Bit> regs(r, Bit(0)); 
    
    std::cout << "Nhip | Input | ";
    for(int i = r - 1; i >= 0; --i) std::cout << "R" << i << " ";
    std::cout << "\n-----------------------------\n";

    for (int step = 0; step < k; ++step) {
        Bit inputBit = message.getBit(step); 
        Bit feedback = inputBit + regs[r - 1]; 
        
        std::vector<Bit> nextRegs(r, Bit(0));
        
        nextRegs[0] = feedback * g.getBit(r); 
        
        for (int i = 1; i < r; ++i) {
            nextRegs[i] = regs[i - 1] + (feedback * g.getBit(r - i));
        }
        
        regs = nextRegs;

        std::cout << std::setw(4) << step + 1 << " | " 
                  << std::setw(5) << inputBit.getVal() << " | ";
        for(int i = r - 1; i >= 0; --i) std::cout << regs[i].getVal() << "  ";
        std::cout << "\n";
    }
    
    BinaryVector codeword = message; 
    BinaryVector remainder(regs);
    
    codeword.append(remainder); 
    
    return codeword;
}
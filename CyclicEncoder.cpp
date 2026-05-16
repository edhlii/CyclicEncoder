#include "CyclicEncoder.h"
#include <iostream>
#include <iomanip>
#include <sstream>

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

std::string CyclicEncoder::encodeJSON(const BinaryVector& message) {
    std::stringstream ss;
    ss << "{";
    ss << "\"n\":" << n << ",\"k\":" << k << ",\"r\":" << r << ",";
    ss << "\"steps\":[";
    
    std::vector<Bit> regs(r, Bit(0)); 
    
    // Nhip 1 den k (khoa K1 dong, K2 mo)
    for (int step = 0; step < k; ++step) {
        Bit inputBit = message.getBit(step); 
        Bit feedback = inputBit + regs[r - 1]; 
        
        std::vector<Bit> nextRegs(r, Bit(0));
        nextRegs[0] = feedback * g.getBit(r); 
        for (int i = 1; i < r; ++i) {
            nextRegs[i] = regs[i - 1] + (feedback * g.getBit(r - i));
        }
        
        if (step > 0) ss << ",";
        ss << "{";
        ss << "\"step\":" << (step + 1) << ",";
        ss << "\"switch\":\"ON\",";
        ss << "\"input\":" << inputBit.getVal() << ",";
        ss << "\"feedback\":" << feedback.getVal() << ",";
        ss << "\"regs\":[";
        for(int i = 0; i < r; ++i) {
            if (i > 0) ss << ",";
            ss << nextRegs[i].getVal();
        }
        ss << "]}";
        
        regs = nextRegs;
    }
    
    // Nhip k+1 den n (khoa K1 mo, K2 dong - shift out)
    for (int step = k; step < n; ++step) {
        std::vector<Bit> nextRegs(r, Bit(0));
        // K1 mo -> feedback = 0
        // Regs shift right
        for (int i = 1; i < r; ++i) {
            nextRegs[i] = regs[i - 1];
        }
        
        ss << ",{";
        ss << "\"step\":" << (step + 1) << ",";
        ss << "\"switch\":\"OFF\",";
        ss << "\"input\":0,";
        ss << "\"feedback\":0,";
        ss << "\"regs\":[";
        for(int i = 0; i < r; ++i) {
            if (i > 0) ss << ",";
            ss << nextRegs[i].getVal();
        }
        ss << "]}";
        
        regs = nextRegs;
    }
    
    ss << "],";
    
    BinaryVector codeword = message; 
    BinaryVector remainder(regs); // Wait, after n steps, regs is empty. The remainder is what was in regs after step k.
    
    // To get the remainder correctly, we need the state of regs after step k.
    // Let's re-encode to get the remainder, or just store it.
    std::vector<Bit> regs_remainder(r, Bit(0)); 
    for (int step = 0; step < k; ++step) {
        Bit inputBit = message.getBit(step); 
        Bit feedback = inputBit + regs_remainder[r - 1]; 
        std::vector<Bit> nextRegs(r, Bit(0));
        nextRegs[0] = feedback * g.getBit(r); 
        for (int i = 1; i < r; ++i) {
            nextRegs[i] = regs_remainder[i - 1] + (feedback * g.getBit(r - i));
        }
        regs_remainder = nextRegs;
    }
    
    BinaryVector rem_vec(regs_remainder);
    codeword.append(rem_vec);
    
    ss << "\"remainder\":[";
    for (int i = 0; i < r; ++i) {
        if (i > 0) ss << ",";
        ss << regs_remainder[i].getVal();
    }
    ss << "],";
    
    ss << "\"codeword\":[";
    for (int i = 0; i < n; ++i) {
        if (i > 0) ss << ",";
        ss << codeword.getBit(i).getVal();
    }
    ss << "]";
    
    ss << "}";
    return ss.str();
}
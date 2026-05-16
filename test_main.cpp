#include <iostream>
#include <string>
#include "BinaryVector.h"
#include "CyclicEncoder.h"

BinaryVector stringToBinaryVector(const std::string& str) {
    std::vector<Bit> bits;
    for (char c : str) {
        if (c == '1') {
            bits.push_back(Bit(1));
        } else if (c == '0') {
            bits.push_back(Bit(0));
        }
    }
    return BinaryVector(bits);
}

int main() {
    int n, k;
    std::string g_str, m_str;

    // Read input
    std::cin >> n >> k >> g_str >> m_str;

    // Convert to BinaryVectors
    BinaryVector g = stringToBinaryVector(g_str);
    BinaryVector m = stringToBinaryVector(m_str);

    // Encode
    CyclicEncoder encoder(n, k, g);
    BinaryVector codeword = encoder.encode(m);

    // Output only the codeword (no debug info, no prompts)
    // We need to output just the binary representation
    for (int i = 0; i < codeword.size(); ++i) {
        std::cout << codeword.getBit(i).getVal();
    }
    std::cout << std::endl;

    return 0;
}

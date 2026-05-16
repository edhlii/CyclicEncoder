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

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " n k g_str m_str" << std::endl;
        return 1;
    }

    int n = std::stoi(argv[1]);
    int k = std::stoi(argv[2]);
    std::string g_str = argv[3];
    std::string m_str = argv[4];

    BinaryVector g = stringToBinaryVector(g_str);
    BinaryVector m = stringToBinaryVector(m_str);

    CyclicEncoder encoder(n, k, g);
    std::cout << encoder.encodeJSON(m) << std::endl;

    return 0;
}
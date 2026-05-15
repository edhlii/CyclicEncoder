#include <iostream>
#include <string>
#include "BinaryVector.h"
#include "CyclicEncoder.h"

// Hàm hỗ trợ chuyển đổi chuỗi String (từ bàn phím) sang BinaryVector
BinaryVector stringToBinaryVector(const std::string& str) {
    std::vector<Bit> bits;
    for (char c : str) {
        if (c == '1') {
            bits.push_back(Bit(1));
        } else if (c == '0') {
            bits.push_back(Bit(0));
        } else {
            std::cout << "Canh bao: Ky tu khong hop le '" << c << "' bi bo qua.\n";
        }
    }
    return BinaryVector(bits);
}

int main() {
    int n, k;
    std::string g_str, m_str;

    std::cout << "===============================================\n";
    std::cout << "   CHUONG TRINH MO PHONG MACH LAP MA VONG\n";
    std::cout << "===============================================\n\n";
    
    // 1. Nhập tham số n và k
    std::cout << "1. Nhap chieu dai tu ma (n): ";
    std::cin >> n;
    std::cout << "2. Nhap chieu dai ban tin (k): ";
    std::cin >> k;

    // 2. Nhập đa thức sinh
    std::cout << "3. Nhap da thuc sinh g(x) dang chuoi bit\n";
    std::cout << "   (Vi du: 1+x+x^3 nhap la 1101): ";
    std::cin >> g_str;
    BinaryVector g = stringToBinaryVector(g_str);

    // 3. Nhập bản tin
    std::cout << "4. Nhap ban tin m(x) dang chuoi bit (chieu dai " << k << "): ";
    std::cin >> m_str;
    
    // Kiểm tra tính hợp lệ của chiều dài bản tin nhập vào
    while (m_str.length() != k) {
        std::cout << "   -> Loi: Chieu dai ban tin phai dung bang k = " << k << "!\n";
        std::cout << "   Vui long nhap lai ban tin m(x): ";
        std::cin >> m_str;
    }
    BinaryVector m = stringToBinaryVector(m_str);

    // --- HIỂN THỊ THÔNG TIN VÀ CHẠY MẠCH ---
    std::cout << "\n================ THONG TIN MA =================\n";
    std::cout << "Ma vong C(" << n << ", " << k << ")\n";
    std::cout << "Da thuc sinh g(x): "; 
    g.print();
    std::cout << "Ban tin m(x)     : "; 
    m.print();
    
    std::cout << "\n======== BANG MO TA HOAT DONG CUA MACH ========\n";
    CyclicEncoder encoder(n, k, g);
    BinaryVector codeword = encoder.encode(m);
    
    std::cout << "\n-----------------------------------------------\n";
    std::cout << "=> Tu ma he thong c(x) thu duoc: ";
    codeword.print();
    std::cout << "===============================================\n";

    return 0;
}
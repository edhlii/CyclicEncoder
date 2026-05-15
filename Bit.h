#pragma once

class Bit {
private:
    bool val;
public:
    Bit(bool v = false);
    
    // Các phép toán trên GF(2)
    Bit operator+(const Bit& other) const;
    Bit operator-(const Bit& other) const;
    Bit operator*(const Bit& other) const;
    Bit operator/(const Bit& other) const;
    
    bool getVal() const;
};
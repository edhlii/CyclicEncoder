#include "Bit.h"

Bit::Bit(bool v) : val(v) {}

Bit Bit::operator+(const Bit& other) const { return Bit(val ^ other.val); }
Bit Bit::operator-(const Bit& other) const { return Bit(val ^ other.val); }
Bit Bit::operator*(const Bit& other) const { return Bit(val & other.val); }
Bit Bit::operator/(const Bit& other) const { return Bit(val & other.val); }

bool Bit::getVal() const { return val; }
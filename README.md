# Cyclic Encoder

A C++ implementation of a cyclic code encoder that simulates error-correcting cyclic code circuits. This project demonstrates the encoding process of cyclic codes, which are an important class of linear codes used in error detection and correction systems.

## Overview

Cyclic codes are linear block codes with the property that every cyclic shift of a codeword is also a codeword. This implementation provides a practical demonstration of how cyclic codes encode messages into codewords using polynomial arithmetic over GF(2) (the Galois Field with two elements).

## Project Structure

```
CyclicEncoder/
├── main.cpp              # Interactive user interface
├── CyclicEncoder.h       # Cyclic encoder class declaration
├── CyclicEncoder.cpp     # Cyclic encoder implementation
├── BinaryVector.h        # Binary vector class declaration
├── BinaryVector.cpp      # Binary vector implementation
├── Bit.h                 # Bit class with GF(2) operations
├── Bit.cpp               # Bit implementation
└── README.md             # This file
```

## Components

### Bit Class
- Represents a single bit (0 or 1)
- Implements arithmetic operations in GF(2) (Galois Field of two elements):
  - Addition (XOR)
  - Subtraction (equivalent to addition in GF(2))
  - Multiplication (AND)
  - Division

### BinaryVector Class
- Represents a vector of bits (polynomials in binary representation)
- Supports:
  - Construction from vectors, size specifications, or initializer lists
  - Bit access and manipulation
  - Vector concatenation (for systematic code construction)
  - Printing/display of binary vectors

### CyclicEncoder Class
- Implements the cyclic encoding algorithm
- Takes parameters:
  - `n`: length of the codeword
  - `k`: length of the message (information bits)
  - `g`: generator polynomial
- Encodes a message into a codeword using polynomial division over GF(2)

## Usage

Run the program and follow the interactive prompts:

```
1. Enter the codeword length (n)
2. Enter the message length (k)
3. Enter the generator polynomial g(x) as a binary string
   - Example: 1+x+x³ is entered as "1101"
4. Enter the message m(x) as a binary string with length k
```

The program will:
- Validate the input lengths
- Display the encoding parameters
- Show the encoding process
- Output the resulting codeword c(x)

### Example

```
Enter codeword length (n): 7
Enter message length (k): 4
Enter generator polynomial g(x) as binary string: 1011
Enter message m(x) of length 4: 1101

Generator polynomial g(x): 1011
Message m(x): 1101
Generated codeword c(x): 1101001
```

## Key Features

- **GF(2) Arithmetic**: Correct implementation of Galois Field arithmetic for polynomial operations
- **Systematic Encoding**: Produces systematic codewords where the message bits appear in the codeword
- **Validation**: Input validation for message length and polynomial format
- **User-Friendly Interface**: Interactive prompts guide users through the encoding process
- **Educational Value**: Clear, well-structured code suitable for learning cyclic code concepts

## Building and Compilation

### Prerequisites
- C++ compiler (C++11 or later)
- Standard library support

### Compilation
```bash
g++ -o cyclic_encoder *.cpp
```

### Running
```bash
./cyclic_encoder
```

## Mathematical Background

Cyclic codes use polynomial arithmetic modulo 2 to encode messages:
- A message m(x) is multiplied by x^(n-k)
- The result is divided by the generator polynomial g(x)
- The remainder is combined with the message to form the codeword

For a C(n, k) cyclic code:
- n = codeword length
- k = message length
- r = n - k = number of parity bits

## Notes

- The program uses Vietnamese language for user interface prompts and output
- All binary arithmetic operations are performed in GF(2)
- The implementation demonstrates both theoretical concepts and practical encoding mechanisms

## Authors

- Le Dinh Hieu: B24DCAT095
- Vo Duy Thang: B24DCAT251
- Nguyen Thanh Dat: B24DCAT049

# CyclicEncoder Test Cases

This directory contains test cases for the CyclicEncoder program.

## Test Structure

- **`N.in`** - Input file for test N, containing 4 space-separated values:
  - `n`: codeword length
  - `k`: message length
  - `g`: generator polynomial (as binary string)
  - `m`: message (as binary string of length k)

- **`N.out`** - Expected output for test N (the encoded codeword)

## Test Cases

### Test 1: Hamming [7,4] Code
- **Input:** n=7, k=4, g(x)=1011, m=1101
- **Expected Output:** 1101100
- **Description:** Standard Hamming [7,4] code with message 1101

### Test 2: All-zeros Message
- **Input:** n=7, k=4, g(x)=1011, m=0000
- **Expected Output:** 0000000
- **Description:** Edge case with all-zero message

### Test 3: Single-bit Message
- **Input:** n=7, k=4, g(x)=1011, m=0001
- **Expected Output:** 0001110
- **Description:** Message with single bit set

### Test 4: All-ones Message
- **Input:** n=7, k=4, g(x)=1011, m=1111
- **Expected Output:** 1111111
- **Description:** Message with all bits set

### Test 5: Larger Code [15,11]
- **Input:** n=15, k=11, g(x)=10011, m=11111111111
- **Expected Output:** 111111111111111
- **Description:** Larger cyclic code with all-ones message

### Test 6: Extended Code [31,26]
- **Input:** n=31, k=26, g(x)=100011, m=11111111111111111111111111
- **Expected Output:** 1111111111111111111111111101000
- **Description:** Extended cyclic code with all-ones message

## Running Tests

### Using PowerShell (Windows)
```powershell
.\run_tests.ps1
```

### Using Batch (Windows)
```batch
run_tests.bat
```

### Manual Testing
```powershell
Get-Content 1.in | ..\test_encoder
```

## Adding New Tests

To add a new test case:

1. Create `N.in` with the input parameters:
   ```
   7 4 1011 1010
   ```

2. Generate expected output:
   ```powershell
   Get-Content N.in | ..\test_encoder | Select-Object -Last 1 | Out-File N.out -Encoding ASCII -NoNewline
   ```

3. Verify the output is correct

4. Add documentation to this file

## Test Program

The test program (`test_main.cpp`) is a simplified version of the main program that:
- Reads input from stdin in format: `n k g m`
- Suppresses interactive prompts and debug output
- Outputs only the final codeword
- Compile with:
  ```
  g++ -o test_encoder ../test_main.cpp ../BinaryVector.cpp ../Bit.cpp ../CyclicEncoder.cpp
  ```

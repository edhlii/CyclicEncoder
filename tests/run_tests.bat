@echo off
REM Test runner script for CyclicEncoder
REM This script runs all test cases and compares outputs with expected results

setlocal enabledelayedexpansion

cd /d "%~dp0"

if not exist "..\test_encoder.exe" (
    echo Compiling test encoder...
    cd ..
    g++ -o test_encoder test_main.cpp BinaryVector.cpp Bit.cpp CyclicEncoder.cpp
    cd tests
    if errorlevel 1 (
        echo Compilation failed!
        exit /b 1
    )
)

echo ===============================================
echo CyclicEncoder Test Suite
echo ===============================================
echo.

set /a passed=0
set /a failed=0
set /a total=0

for /l %%i in (1,1,6) do (
    set /a total=!total!+1
    
    echo Test %%i: 
    type %%i.in
    
    REM Run test and capture output
    ..\test_encoder < %%i.in > temp_%%i.out 2>&1
    
    REM Extract only the last line (the codeword)
    for /f "tokens=* usebackq" %%a in (`findstr /N "^" temp_%%i.out ^| find /v "Nhip" ^| find /v "---" ^| find /v "Input"`) do set last_line=%%a
    
    echo Expected: 
    type %%i.out
    echo.
    
    set /a failed=!failed!+1
    echo [FAILED]
    echo.
)

echo ===============================================
echo Test Results: !passed! passed, !failed! failed (out of !total! tests)
echo ===============================================

endlocal

#!/usr/bin/env pwsh

# Test runner script for CyclicEncoder
# This script runs all test cases and compares outputs with expected results

$testDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
$rootDir = Split-Path -Parent $testDir

# Compile test encoder if needed
if (-not (Test-Path "$rootDir\test_encoder.exe")) {
    Write-Host "Compiling test encoder..." -ForegroundColor Cyan
    Push-Location $rootDir
    g++ -o test_encoder test_main.cpp BinaryVector.cpp Bit.cpp CyclicEncoder.cpp
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Compilation failed!" -ForegroundColor Red
        exit 1
    }
    Pop-Location
}

Write-Host "===============================================" -ForegroundColor Cyan
Write-Host "CyclicEncoder Test Suite" -ForegroundColor Cyan
Write-Host "===============================================" -ForegroundColor Cyan
Write-Host ""

$passed = 0
$failed = 0
$total = 0

for ($i = 1; $i -le 6; $i++) {
    $total++
    
    $inputFile = "$testDir\$i.in"
    $expectedFile = "$testDir\$i.out"
    
    if (-not (Test-Path $inputFile) -or -not (Test-Path $expectedFile)) {
        Write-Host "Test ${i}: [SKIPPED - Files not found]" -ForegroundColor Yellow
        continue
    }
    
    Write-Host "Test ${i}:" -ForegroundColor Green
    $input = Get-Content $inputFile
    Write-Host "  Input: $input"
    
    # Run test and capture output, extracting just the codeword
    $output = Get-Content $inputFile | & "$rootDir\test_encoder" 2>$null | Select-Object -Last 1
    
    $expected = Get-Content $expectedFile
    Write-Host "  Expected: $expected"
    Write-Host "  Got:      $output"
    
    if ($output -eq $expected) {
        Write-Host "  [PASSED]" -ForegroundColor Green
        $passed++
    } else {
        Write-Host "  [FAILED]" -ForegroundColor Red
        $failed++
    }
    Write-Host ""
}

Write-Host "===============================================" -ForegroundColor Cyan
Write-Host "Test Results: $passed passed, $failed failed (out of $total tests)" -ForegroundColor Cyan
Write-Host "===============================================" -ForegroundColor Cyan

# Count-Min Sketch

A C++17 implementation of the Count-Min Sketch (CMS), a probabilistic data structure for estimating item frequencies in large data streams using fixed memory.

The project includes the CMS implementation, automated tests, and benchmark scripts for evaluating accuracy across different sketch widths.

## Features

* Count-Min Sketch implementation in C++
* Configurable width and depth
* 64-bit integer counters
* Deterministic benchmark data using a fixed random seed
* Tests for the no-underestimation property
* Accuracy and error measurements
* Benchmark scripts for comparing different widths
* Automated build and test workflow using CMake and CTest

## Requirements

* C++ compiler with C++17 support
* CMake
* Make or another supported CMake build tool
* CTest

## Build and Test

The project provides a build script that configures CMake, builds the project, and runs the test suite.

```bash
./build.sh
```

This performs:

```text
cmake configuration
      ↓
build
      ↓
run tests with CTest
```

## Running the Test Program

The test executable accepts two optional arguments:

```text
cms_test [N] [WIDTH]
```

Where:

* `N` is the number of items in the generated stream
* `WIDTH` is the width of the Count-Min Sketch

For example:

```bash
./build/test/cms_test 10000000 5000
```

If no arguments are provided, the test program uses its configured default values.

## Benchmark Scripts

### Single Test

`run_test.sh` runs the CMS test for a specified stream size and width.

```bash
./run_test.sh 10000000 5000
```

### Width Comparison — Detailed

`run_widths.sh` runs the benchmark across multiple widths and displays the complete output for each test.

```bash
./run_widths.sh
```

### Width Comparison — Summary

`run_widths_summary.sh` runs the benchmark across multiple widths and displays a compact summary containing exact-count accuracy and average error.

```bash
./run_widths_summary.sh
```

The stream size can be provided as the first argument:

```bash
./run_widths_summary.sh 50000000
```

Without an argument, the summary script uses a default stream size of `10,000,000`.

Example output:

```text
Width      Exact    Avg Error   
-------------------------------------
500        0.00%    14428.29
2000       2.96%    2583.87 
5000       50.72%   565.12  
10000      89.70%   100.65  
20000      99.06%   9.14    
50000      100.00%  0.00    
100000     100.00%  0.00
```

## Test Configuration

The benchmark currently uses:

| Parameter                     |                                              Value |
| ----------------------------- | -------------------------------------------------: |
| Depth                         |                                                  5 |
| Key range                     |                                             10,000 |
| Random seed                   |                                          123456789 |
| Default benchmark stream size |                                         10,000,000 |
| Tested widths                 | 500, 2,000, 5,000, 10,000, 20,000, 50,000, 100,000 |

The benchmark uses a fixed seed so that results are reproducible across runs with the same configuration.

## Project Structure

```text
.
├── include/
│   └── cms.hpp
├── src/
│   └── cms.cpp
├── test/
│   └── ...
├── build.sh
├── run_test.sh
├── run_widths.sh
├── run_widths_summary.sh
├── CMakeLists.txt
└── README.md
```

The `build/` directory is generated during the build and is excluded from version control.

## Development Workflow

After modifying C++ source files, headers, CMake configuration, or tests:

```bash
./build.sh
```

After the tests pass, benchmark changes with:

```bash
./run_widths_summary.sh
```

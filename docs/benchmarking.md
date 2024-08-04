# Benchmarking Guide

## Introduction

Benchmarking is an essential part of software development to measure the performance and efficiency of your code. In this guide, we will learn how to run and write benchmark suites using ScribbleLabApps's benchh library.

## Getting Started

### Requirements

Before starting to run or write benchmark suites make sure you have the following dependencies installed:

- **Clang**: Version ≥ 16.0
- **CMake**: Version ≥ 3.10
- **Ninja**: Version ≥ 1.12.1

If you don't have the following dependencies installed feel free to download them either manually or with our [`Brewfile`](https://github.com/ScribbleLabApp/SFFileManagementKit/blob/main/Brewfile).

### Setting up environment & Run Benchmark Suites

To start benchmarking SFFileManagementKit, follow these steps:

1. Clone the SFFileManagementKit repository to your local machine.

2. Navigate to the `script` directory of the repository.

3. Give our pre-made benchmark.sh script the necessary permissions to run:

    ```shell
    $ chmod u+x benchmark.sh
    ```
   
4. Finally run our benchmark.sh script with the following command:

    ```shell
    $ ./benchmark.sh
    ```

    This will ***(i)*** compile the benchmark library and suites and ***(ii)*** run all the benchmark test cases provided by `bchsuite.h`.

### Writing Benchmark Tests

Now that you have set up the necessary files, you can start writing benchmark tests.

1. Open the `Tests/Benchmarks/` folder in an editor of your choice

#### Benchmark Folder Structure

- `main.c`: Main entry point for the SFBenchmark Suite
- `CMakeLists.txt`: Build instructions for CMake
- `include/bench.h`: ScribbleLabApp's Benchmark Library
- `include/bchsuite.h`: Main Benchmark Test Suite

2. To create a new benchmark test case simply add a function to the `bchsuite.h` header file.

```c++
//===-- Benchmarks/include/bchsuite.h - Benchmarking --------------*- C -*-===//
//                                                                            //
// This source file is part of the Scribble Foundation open source project    //
//                                                                            //
// Copyright (c) 2024 ScribbleLabApp. and the ScribbleLab project authors     //
// Licensed under Apache License v2.0 with Runtime Library Exception          //
//                                                                            //
// Copyright (c) 2022 Olaf Berstein                                           //
//                                                                            //
// You may not use this file except in compliance with the License.           //
// You may obtain a copy of the License at                                    //
//                                                                            //
//      http://www.apache.org/licenses/LICENSE-2.0                            //
//                                                                            //
// Unless required by applicable law or agreed to in writing, software        //
// distributed under the License is distributed on an "AS IS" BASIS,          //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   //
// See the License for the specific language governing permissions and        //
// limitations under the License.                                             //
//                                                                            //
//===----------------------------------------------------------------------===//

#ifndef BCHSUITE_H
#define BCHSUITE_H

// Import benchmark macros & functions
#include "bench.h"

// Add your tests cases below this comment

#endif /* BCHSUITE_H */
```

Scribble Benchmark Test cases are simple functions that contains the to be benchmarked implementations or methods used for that test case.

```c++
// Replace this function with your actual benchmark test implementation
void test(void) {
    BENCH("Example Function Benchmark", 10, 100);
    for (volatile int i = 0; i < 1000000; ++i);
}
```

In the example, the `test() function is used to benchmark the performance of a specific function or method.
In this example, the benchmark test case is named "Example Function Benchmark". It performs 10 warm-up iterations and 100 benchmark iterations. The benchmarked code is a simple loop that iterates 1,000,000 times.
The volatile keyword is used to ensure that the loop variable i is not optimized away by the compiler. This is important for accurate benchmarking, as the compiler might otherwise optimize the loop away.

> [!Warning]
> Do not return any additional code beyond the immediate scope of the code block. This ensures that the benchmark test case is properly defined and isolated from other code.

<details>
<summary>Scribble Benchmarking Macros & Functions</summary>

#### Scribble Benchmarking Macros & Functions

|                        Method                        | Type |                                                                                        Description                                                                                        |
|:----------------------------------------------------:|:----:|:-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|
|                  `BENCH_CLOBBER()`                   |  M   | Creates an artificial use of all memory to prevent the compiler from optimizing out memory operations. It ensures that the compiler does not assume that memory operations are redundant. |
|                 `BENCH_VOLATILE(x)`                  |  M   |              Creates an artificial dependency between `x` and all memory operations. This prevents the compiler from reordering or optimizing out operations involving `x`.               |
|               `BENCH_VOLATILE_REG(x)`                |  M   |                                                           Similar to `BENCH_VOLATILE`, but specifically for register variables.                                                           |
|               `BENCH_VOLATILE_MEM(x)`                |  M   |                                                        Similar to `BENCH_VOLATILE`, but specifically for memory-mapped variables.                                                         |
|               `BENCH_CLOBBER_WITH(x)`                |  M   |                                   Ensures that the variable `x` is used in a way that prevents compiler optimizations and also calls `bench__use_ptr`.                                    |
|             `BENCH_CLOBBER_WITH_REG(x)`              |  M   |                                                               Similar to `BENCH_CLOBBER_WITH`, but for register variables.                                                                |
|             `BENCH_CLOBBER_WITH_MEM(x)`              |  M   |                                                             Similar to `BENCH_CLOBBER_WITH`, but for memory-mapped variables.                                                             |
|                `bench_gettime(void)`                 |  fn  |             Returns the current time in seconds. It uses different clock sources depending on the platform (`CLOCK_PROCESS_CPUTIME_ID`, `CLOCK_MONOTONIC_RAW`, or `clock()`).             |
|          `bench_append(char const *title)`           |  fn  |                                 Appends a new benchmark record with the given `title` to the internal benchmark data. Allocates more memory if necessary.                                 |
|             `bench_update(double time)`              |  fn  |                                  Updates the most recent benchmark record with the provided `time`. Computes mean, standard deviation, and minimum time.                                  |
| `bench_record_cmp(void const *lhs, void const *rhs)` |  fn  |                                                 Comparison function for sorting benchmark records by their mean time in ascending order.                                                  |
|                  `bench_done(void)`                  |  fn  |                        Finalizes the benchmarking process by sorting records and printing the results, including mean time, standard deviation, and minimum time.                         |
|                  `bench_free(void)`                  |  fn  |                                                                     Frees the memory allocated for benchmark records.                                                                     |
|              `bench_hash64(uint64_t x)`              |  fn  |                                       Hashes a 64-bit integer using a custom hashing algorithm. Useful for generating hash values for benchmarking.                                       |

</details>

3. Open `main.c` and finally call your benchmark test case from the main function.

```c++
//===-- Benchmarks/main.c - Benchmarking --------------------------*- C -*-===//
//                                                                            //
// This source file is part of the Scribble Foundation open source project    //
//                                                                            //
// Copyright (c) 2024 ScribbleLabApp. and the ScribbleLab project authors     //
// Licensed under Apache License v2.0 with Runtime Library Exception          //
//                                                                            //
// Copyright (c) 2022 Olaf Berstein                                           //
//                                                                            //
// You may not use this file except in compliance with the License.           //
// You may obtain a copy of the License at                                    //
//                                                                            //
//      http://www.apache.org/licenses/LICENSE-2.0                            //
//                                                                            //
// Unless required by applicable law or agreed to in writing, software        //
// distributed under the License is distributed on an "AS IS" BASIS,          //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   //
// See the License for the specific language governing permissions and        //
// limitations under the License.                                             //
//                                                                            //
//===----------------------------------------------------------------------===//

#include <stdio.h>
#include <stdlib.h>

// Import benchmark function suite
#include "bchsuite.h"

// Import benchmark macros & functions
#include "bench.h"

int main(void) {
    printf("Running benchmark test...\n");

    // Add benchmark test functions below this comment
    test(); /* Call the "Example Function Benchmark" test case */

    bench_done();
    bench_free();

    return 0;
}
```
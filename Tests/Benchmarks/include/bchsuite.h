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
///
/// \file
///
//===----------------------------------------------------------------------===//

#ifndef BCHSUITE_H
#define BCHSUITE_H

// Import benchmark macros & functions
#include "bench.h"

// Replace this function with your actual benchmark test implementation
void test(void) {
    //printf("Running benchmark test for 'test'\n");
    //printf("Benchmark test completed. Time taken: %.9f seconds\n", end_time - start_time);

    BENCH("Example Function Benchmark", 10, 100);
    for (volatile int i = 0; i < 1000000; ++i);
}

#endif //BCHSUITE_H

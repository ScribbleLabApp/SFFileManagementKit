//===-- SFFileCoreASM/include/crc32.h - CRC32 AlG --------------  -*- C -*-===//
//                                                                            //
// This source file is part of the Scribble Foundation open source project    //
//                                                                            //
// Copyright (c) 2024 ScribbleLabApp. and the ScribbleLab project authors     //
// Licensed under Apache License v2.0 with Runtime Library Exception          //
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
/// This file declares the `crc32` function implemented in assembly. The function
/// calculates the CRC32 checksum for the given data buffer.
///
//===----------------------------------------------------------------------===//

#ifndef CRC32_H
#define CRC32_H

#include <stdint.h>
#include <Kernel/sys/_types/_size_t.h>

/**
 * @brief Lookup table for CRC32 calculation.
 *
 * This static table contains precomputed CRC32 values for each possible byte value
 * (0-255). It is used to speed up the CRC32 checksum calculation by avoiding
 * recomputation of CRC values for each byte during the checksum process.
 * 
 * The table is initialized by the `crc32_init` function, which computes the CRC
 * values using the polynomial 0xEDB88320.
 */
static uint32_t crc32_table[256];

/**
 * @brief Calculate the CRC32 checksum of a data buffer.
 *
 * This function calculates the CRC32 checksum for a given data buffer using
 * the `crc32_table` lookup table for efficient computation.
 *
 * @param data Pointer to the data buffer.
 * @param length Length of the data buffer in bytes.
 * @return CRC32 checksum.
 */
extern uint32_t crc32(const uint8_t *data, size_t length);

/**
 * @brief Initialize the CRC32 lookup table.
 *
 * This function initializes the `crc32_table` with precomputed CRC32 values
 * for each possible byte value (0-255) using the polynomial 0xEDB88320.
 * It must be called before using the `crc32` function to ensure that
 * the lookup table contains valid data.
 */
void crc32_init(void) {
    uint32_t crc;
    for (uint32_t i = 0; i < 256; i++) {
        crc = i;
        for (uint32_t j = 8; j > 0; j--) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320;
            } else {
                crc >>= 1;
            }
        }
        crc32_table[i] = crc;
    }
}

#endif /* CRC32_H */

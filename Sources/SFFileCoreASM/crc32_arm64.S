;===-- SFFileCoreASM/crc32_arm64.s - CRC32 Algorithm ------------------------===;
;                                                                                ;
; This source file is part of the Scribble Foundation open source project        ;
;                                                                                ;
; Copyright (c) 2024 ScribbleLabApp. and the ScribbleLab project authors         ;
; Licensed under Apache License v2.0 with Runtime Library Exception              ;
;                                                                                ;
; You may not use this file except in compliance with the License.               ;
; You may obtain a copy of the License at                                        ;
;                                                                                ;
;      http://www.apache.org/licenses/LICENSE-2.0                                ;
;                                                                                ;
; Unless required by applicable law or agreed to in writing, software            ;
; distributed under the License is distributed on an "AS IS" BASIS,              ;
; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.       ;
; See the License for the specific language governing permissions and            ;
; limitations under the License.                                                 ;
;                                                                                ;
;===--------------------------------------------------------------------------===;

.section __DATA,__data
    .extern _crc32_table                        ; Reference to the CRC32 lookup table (declared in `crc32.h`)

.section __TEXT,__text
    .globl _crc32                               ; Make the 'crc32' function available globally

_crc32:                                         ; Arguments: x0 = data pointer, x1 = length
    mov x2, #0xFFFFFFFF                         ; Initialize x2 with 0xFFFFFFFF (starting value for CRC calculation)
    mov x3, x0                                  ; Move data pointer to x3
    mov x4, x1                                  ; Move length to x4

.loop:
    ldrb w5, [x3]                               ; Load the next byte of data into w5
    eor w2, w2, w5                              ; XOR the byte with the CRC value in w2
    lsr w2, w2, #8                              ; Shift right by 8 bits
    ldr w6, [x2, w2, uxtw #2]                   ; Load CRC32 lookup table entry into w6
    eor w2, w2, w6                              ; XOR with the CRC32 table entry

    add x3, x3, #1                              ; Move to the next byte in the input data
    subs x4, x4, #1                             ; Decrement the length counter
    b.ne .loop                                  ; If length counter is not zero, repeat the loop
    mvn x2, x2                                  ; Invert all the bits in x2 (final step of CRC32)
    ret                                         ; Return with the result in x2

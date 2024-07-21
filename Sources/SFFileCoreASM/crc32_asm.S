;===-- SFFileCoreASM/crc32_asm.S - CRC32 AlG --------------------  -*- asm -*-===;
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

#include "crc32.h"

section .data
    extern crc32_table     ; Reference to the CRC32 lookup table (declared in `crc32.h`)

section .text
    global crc32           ; Make the 'crc32' function available globally

crc32:
    ; Arguments: rdi = data pointer, rsi = length
    push rbx               ; Save the value of rbx register on the stack
    mov eax, 0xFFFFFFFF    ; Initialize eax with 0xFFFFFFFF (starting value for CRC calculation)

.loop:
    movzx bl, byte [rdi]   ; Load the next byte of data into bl, zero-extended to 32 bits
    xor al, bl             ; XOR the byte with the lowest byte of eax
    shr eax, 8             ; Shift eax right by 8 bits (drop the lowest byte)
    xor eax, dword [crc32_table + rax * 4]
                           ; XOR eax with the CRC32 lookup table entry corresponding to the dropped byte
    inc rdi                ; Move to the next byte in the input data
    dec rsi                ; Decrement the length counter
    jnz .loop              ; If length counter is not zero, repeat the loop

    not eax                ; Invert all the bits in eax (final step of CRC32)
    pop rbx                ; Restore the original value of rbx register
    ret                    ; Return with the result in eax

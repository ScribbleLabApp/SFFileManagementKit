# ==-- SFFileCoreASM/crc32_x86.S - CRC32 Algorithm --------------  -*- asm -*-== #
#                                                                                #
# This source file is part of the Scribble Foundation open source project        #
#                                                                                #
# Copyright (c) 2024 ScribbleLabApp. and the ScribbleLab project authors         #
# Licensed under Apache License v2.0 with Runtime Library Exception              #
#                                                                                #
# You may not use this file except in compliance with the License.               #
# You may obtain a copy of the License at                                        #
#                                                                                #
#      http://www.apache.org/licenses/LICENSE-2.0                                #
#                                                                                #
# Unless required by applicable law or agreed to in writing, software            #
# distributed under the License is distributed on an "AS IS" BASIS,              #
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.       #
# See the License for the specific language governing permissions and            #
# limitations under the License.                                                 #
# ==--------------------------------------------------------------------------== #

.section .data
.extern crc32_table                     # Reference to the CRC32 lookup table

.section .text
.globl _crc32
_crc32:                                 # Arguments: %rdi = data pointer, %rsi = length

    pushq %rbx                          # Save the value of %rbx register on the stack
    movl $0xFFFFFFFF, %eax              # Initialize %eax with 0xFFFFFFFF (starting value for CRC calculation)
    
.loop:
    movzbl (%rdi), %ebx                 # Load the next byte of data into %ebx, zero-extended to 32 bits
    xorb %bl, %al                       # XOR the byte with the lowest byte of %eax
    shrl $8, %eax                       # Shift %eax right by 8 bits (drop the lowest byte)
    movl %eax, %ecx                     # Move %eax to %ecx
    movl crc32_table(,%ecx,4), %ecx     # Load CRC32 lookup table entry into %ecx
    xorl %ecx, %eax                     # XOR %eax with the CRC32 table entry
    addq $1, %rdi                       # Move to the next byte in the input data
    subq $1, %rsi                       # Decrement the length counter
    jnz .loop                           # If length counter is not zero, repeat the loop
    
    notl %eax                           # Invert all the bits in %eax (final step of CRC32)
    popq %rbx                           # Restore the original value of %rbx register
    ret                                 # Return with the result in %eax

section .data
    extern crc32_table     ; Reference to the CRC32 lookup table (declared in another module)

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

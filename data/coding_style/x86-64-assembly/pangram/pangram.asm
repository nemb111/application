
section .text
global is_pangram
is_pangram:

    xor eax, eax
    xor ebx, ebx
    

    .loop:
    movzx ecx, byte [rdi]
    cmp cl, 0
    je .exit

    and cl, 0b1101_1111  ;  transform the ascii characters to upper case  
    sub cl, 'A'          ;  Normalize to interval [0, 'Z' - 'A']
    cmp cl,  'Z' - 'A'
    setbe bl
    add cl, 1            ;  transform interval to [1, 27] to use bit 1-27 to keep track of the chars. Bit 0 is set if cl does not contain a valid char
    imul cx, bx          ;  cl is set to 0 if it doesn't hold a valid char. Otherwise it stays unchanged.
    bts eax, ecx

    inc rdi
    jmp .loop


    .exit:
    bts eax, 0            ; Set bit 0 explicitly so we can test for 27 set bit
    cmp eax, 0x7FF_FFFF   ; Check if all 27 lower bits are set
    sete al
    movzx eax, al

    ret



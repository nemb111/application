section .text
global leap_year


; Checks if the given number is even divisible by another given number. The result is stored in a byte register
; %1 r64: Containing the dividend to be checked for even divisibility
; %2 imm: The divisor for even divisibility check
; Post : ZF is set if true, otherwise ZF is not set 
%macro  is_even_divisible  2

    mov rax, %1
    xor rdx, rdx
    mov r8, %2
    div r8
    test edx, edx

%endmacro


leap_year:
    
    ; if year % 4 == 0 && (!(year % 100 == 0) || year % 400 == 0) => true,
    ;                                                   otherwise => false
    

    ; A: Handle case: A:= year % 4 == 0
    is_even_divisible rdi, 4
    mov rax, 0
    jnz .exit

    ; Handle case: B:= !(year % 100 == 0)
    is_even_divisible rdi, 100
    mov rax, 1
    jnz .exit

    ; Handle case: C:= year % 400 == 0
    is_even_divisible rdi, 400
    setz al


 
    .exit:
    ret

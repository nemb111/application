section .text
global square_of_sum
square_of_sum:
    ; => ((n+1)*n/2)^2
    mov rax, rdi
    add rax, 1
    mul rdi
    mul rax
    shr rax, 2
    ret

global sum_of_squares
sum_of_squares:
    ; => n(n+1)(2n+1) / 6
    mov rax, rdi
    add rax, rax
    add rax, 1
    add rdi, 1
    mul rdi
    sub rdi, 1
    mul rdi
    mov r8d, 6
    div r8
    ret

global difference_of_squares
difference_of_squares:
    call sum_of_squares
    mov rcx, rax
    call square_of_sum
    sub rax, rcx
    ret

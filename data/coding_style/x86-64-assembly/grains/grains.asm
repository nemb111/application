section .text
global square
square:
    sub rdi, 1
    
    xor rax, rax
    bts rax, rdi

    xor rbx, rbx
    cmp rdi, 63
    cmova rax, rbx

    ret

global total
total:
    mov rax, 0xFF_FF_FF_FF_FF_FF_FF_FF
    ret


section .text
global to_rna
to_rna:

    .loop:
    cmp byte [rdi], 0
    je .exit
    cmp byte [rdi], 'A'
    mov cx, 'U'
    cmove rax, rcx
    cmp byte [rdi], 'C'
    mov cx, 'G'
    cmove rax, rcx
    cmp byte [rdi], 'G'
    mov cx, 'C'
    cmove rax, rcx
    cmp byte [rdi], 'T'
    mov cx, 'A'
    cmove rax, rcx

    mov [rsi], al
    inc rdi
    inc rsi
    mov byte [rsi], 0

    jmp .loop
    

    .exit:
    ret

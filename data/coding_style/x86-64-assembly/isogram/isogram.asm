section .text
global is_isogram
is_isogram:
    mov rax, 1
    xor rbx, rbx
    xor rdx, rdx

    .loop_start:
    mov bl, byte [rdi]
    cmp bl, 0
    je .exit
    add rdi, 1
    and bl, 0b1101_1111
    sub bl, 'A'
    cmp bl, 'Z' - 'A'
    jbe .checkForRepeatedCharachter
    jmp .loop_start

    .exit:
    ret


.exit_false:
    xor rax, rax
    jmp .exit


.checkForRepeatedCharachter:
    bts rdx, rbx
    jc .exit_false
    jmp .loop_start

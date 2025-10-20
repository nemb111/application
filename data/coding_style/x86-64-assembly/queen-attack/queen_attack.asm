section .text
global can_create
can_create:
        xor rax, rax
        cmp rdi, 7
        ja .return
        cmp rsi, 7
        ja .return

        mov rax, 1
    .return:
        ret

global can_attack
can_attack:
        mov rax, 1

        ; Let the indices be from 1 to 8, this avoids division by zero when the diagonal is checked.
        inc rdi
        inc rsi
        inc rdx
        inc rcx

        ; Check if the queens are on the same row
        cmp rdi, rdx
        je .return

        ; Check if the queens are on the same column
        cmp rsi, rcx
        je .return

        ; Check if the queens are on the same diagonal
        mov r8, rdi
        sub r8, rdx
        mov r9, rsi
        sub r9, rcx
        ; Calculate the slope which needs to be 1 or -1
        mov r10, rax
        mov rax, r9
        cqo
        idiv r8
        mov r9, rax
        mov rax, r8
        cqo
        idiv r8
        mov r8, rax
        mov rax, r10
        imul r8, r9
        imul r8, r8
        test r8, 1
        jnz .return

        ; If we reach here, the queens are not on the same row, column or diagonal
        xor rax, rax

    .return:
        ret

%ifidn __OUTPUT_FORMAT__,elf64
section .note.GNU-stack noalloc noexec nowrite progbits
%endif

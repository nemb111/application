section .text

; Hamming distance calculation
; Parameters:
;   rdi - pointer to first string
;   rsi - pointer to second string
global distance
distance:
        xor rax, rax 
        xor rcx, rcx 

    .loop:
        movzx r8, byte [rdi + rcx]
        movzx r9, byte [rsi + rcx]
        test r8, r8
        jz .end

        cmp r8, r9
        setnz r10b
        inc rcx
        add rax, r10
        jmp .loop

    .return:
        ret

distance.end:
    test r9, r9
    jz distance.return

    mov rax, -1
    jmp distance.return


%ifidn __OUTPUT_FORMAT__,elf64
section .note.GNU-stack noalloc noexec nowrite progbits
%endif

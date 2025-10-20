section .text
global allergic_to
allergic_to:
    mov cl, dil
    shr rsi, cl
    and rsi, 0b1
    setnz al
    movzx rax, al
    ret

global list
list:
    and rdi, 0xFF ; Ensure rdi is within the range of 0-255
    xor rax, rax 
    xor rcx, rcx 
    lea rsi, [rsi + 4] ; Adjust rsi to point to the first element of the list (skip integer count)
    .loop:    
        cmp rdi, 0
        jz .end
        test rdi, 1
        jnz .store_back
    .store_back_ret:

        shr rdi, 1
        inc rcx 

        jmp .loop

    .end:
        lea rsi, [rsi - 4]
        mov [rsi], dword eax ; Store the count of elements in the list at the start
        ret

list.store_back:
    mov [rsi + rax * 4], dword ecx 
    inc rax 
    jmp list.store_back_ret

%ifidn __OUTPUT_FORMAT__,elf64
section .note.GNU-stack noalloc noexec nowrite progbits
%endif

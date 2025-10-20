section .text

; collatz_conjecture
; Macro to perform Collatz operation on a register.
; %1 - register containing the current value (eax is recommended).
; Returns eax - next number in collatz conjecture.
%macro collatz_conjecture 1
    %ifnidni %1, eax
        mov eax, %1
    %endif
        ; Case for even number
        test eax, 1
        jz %%is_even

        ; Handling the odd case
        imul eax, 3
        add  eax, 1
        jmp %%end

    %%is_even:
        shr eax, 1

    %%end:
%endmacro

global steps
steps:
    ; Handle the case input is zero or negative
    test edi, edi
    js end_error
    jz end_error

    ; Handle the case input is 1 which means already done
    mov eax, edi
    cmp edi, 1
    je end
    
    mov ecx, 1

loop_start:
    collatz_conjecture eax
    inc ecx
    cmp eax, 1 
    jne loop_start

    mov eax, ecx

end: 
    dec eax
    ret

end_error:
    mov eax, -1
    ret

%ifidn __OUTPUT_FORMAT__,elf64
section .note.GNU-stack noalloc noexec nowrite progbits
%endif

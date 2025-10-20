section .text

; a, b, c are the lengths of the sides of the triangle.
; They are passed in xmm0, xmm1, and xmm2 respectively.
; Parameters:
;   xmm0 RO - first side length
;   xmm1 RO - second side length
;   xmm2 RO - third side length
; Sideeffects:
;   xmm3 RW - temporary storage for calculations
; Return value:
;   rax RW - 1 if the triangle is valid, 0 otherwise
%macro is_triangle 3
    %ifnidn %1, xmm0
        %error "First argument must be xmm0"
    %endif
    %ifnidn %2, xmm1
        %error "Second argument must be xmm1"
    %endif
    %ifnidn %3, xmm2
        %error "Third argument must be xmm2"
    %endif

    xor rax, rax ; Clear rax to 0

    ; Check if a, b, c are all positive
    xorps xmm3, xmm3
    ucomisd xmm0, xmm3
    jbe %%end_false
    ucomisd xmm1, xmm3
    jbe %%end_false
    ucomisd xmm2, xmm3
    jbe %%end_false

    ; Check if a + b ≥ c
    movaps xmm3, %1
    addsd xmm3, %2
    ucomisd xmm3, xmm2
    jb %%end_false

    ; Check if b + c ≥ a
    movaps xmm3, %2
    addsd xmm3, %3
    ucomisd xmm3, %1
    jb %%end_false

    ; Check if a + c ≥ b
    movaps xmm3, %1
    addsd xmm3, %3
    ucomisd xmm3, %2
    jb %%end_false

    %%end_true:
    mov rax, 1
    jmp %%end

    %%end_false:
    %%end:
%endmacro

global is_equilateral
is_equilateral:
        movsd xmm0, [rsp + 8] 
        movsd xmm1, [rsp + 16] 
        movsd xmm2, [rsp + 24] 


        is_triangle xmm0, xmm1, xmm2
        cmp rax, 1
        jne .not_equilateral
        
        ; Check if all sides are equal
        ucomisd xmm0, xmm1
        jne .not_equilateral
        ucomisd xmm1, xmm2
        jne .not_equilateral
        ucomisd xmm0, xmm2
        jne .not_equilateral


    .return:
        ret

    .not_equilateral:
        xor rax, rax ; Set rax to 0
        jmp .return



global is_isosceles
is_isosceles:
        movsd xmm0, [rsp + 8] 
        movsd xmm1, [rsp + 16] 
        movsd xmm2, [rsp + 24] 

        is_triangle xmm0, xmm1, xmm2
        cmp rax, 1
        jne .not_isosceles

        ; Check if two sides are equal
        ucomisd xmm0, xmm1
        je .return
        ucomisd xmm1, xmm2
        je .return
        ucomisd xmm0, xmm2
        je .return
        ; If none of the sides are equal, it's not isosceles
        jmp .not_isosceles

    .return:    
        ret

    .not_isosceles:
        xor rax, rax ; Set rax to 0
        jmp .return

global is_scalene
is_scalene:
        movsd xmm0, [rsp + 8] 
        movsd xmm1, [rsp + 16] 
        movsd xmm2, [rsp + 24] 

        is_triangle xmm0, xmm1, xmm2
        cmp rax, 1
        jne .not_scalene

        ; Check if any sides are equal
        ucomisd xmm0, xmm1
        je .not_scalene
        ucomisd xmm1, xmm2
        je .not_scalene
        ucomisd xmm0, xmm2
        je .not_scalene

    .return:    
        ret

    .not_scalene:
        xor rax, rax ; Set rax to 0
        jmp .return

%ifidn __OUTPUT_FORMAT__,elf64
section .note.GNU-stack noalloc noexec nowrite progbits
%endif
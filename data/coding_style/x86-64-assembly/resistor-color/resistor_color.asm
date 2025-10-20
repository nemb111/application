
section .rodata
    black: db "black",0
    brown: db "brown",0
    red: db "red",0
    orange: db "orange",0
    yellow: db "yellow",0
    green: db "green",0
    blue: db "blue",0
    violet: db "violet",0
    grey: db "grey",0
    white: db "white",0
    colors_arr: dq black, brown, red, orange, yellow, green, blue, violet, grey, white, 0


section .text
global color_code

%define NUM_OF_COLORS 10

%macro compare_string 2

    mov r8, %1
    lea r9, [rel %2]

    %%.loop:
    mov r10b, byte [r9]
    cmp r10b, byte [r8]
    jne %%.end_compare_string

    add r8, 1
    add r9, 1

    test r10b, r10b
    jnz %%.loop

    %%.end_compare_string:

%endmacro

color_code:

    xor rax, rax
    lea rbx, [rel colors_arr]

    .loop:

    mov rcx, [rel rbx]
    compare_string rdi, rcx
    jz .end

    cmp eax, NUM_OF_COLORS-1
    jae .end

    add eax, 1
    add rbx, 8
    jmp .loop


    .end:
    ret

global colors
colors:
    lea rax, [rel colors_arr]
    ret

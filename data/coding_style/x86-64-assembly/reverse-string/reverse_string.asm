section .text
global reverse

reverse:
    cmp byte [rdi], 0
    je .empty_str
    
	call count_number_of_char

	lea rsi, [rdi+rax-1]

	.loop:
	mov al, [rdi]
	mov cl, [rsi]
	mov [rdi], cl
	mov [rsi], al
	add rdi, 1
	sub rsi, 1
	cmp rdi, rsi
	jl .loop
    
    .empty_str:
    ret

    
count_number_of_char:
	xor rax, rax

	.loop:
	cmp byte [rdi + rax], 0
	je .end
	add rax, 1
	jmp .loop

	.end:
	ret

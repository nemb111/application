
section .rodata
msg: db "One for ", 0
msg2: db "you", 0
msg3: db ", one for me.", 0

section .text

global two_fer


; rdi start address of string to copy into dest
; rsi start address of destination buffer
; rax return number of written non-null characters
copystr:
  xor rax, rax
  mov cl, [rdi]            
  test cl, cl                
  jle .loop_end           
  .loop_start:
    add rax, 1
    add rdi, 1                 
    mov [rsi], cl           
    add rsi, 1
    mov cl, [rdi]            
    test cl, cl                 
    jg .loop_start

  .loop_end:
    mov [rsi], byte 0
    ret

two_fer:
  push rdi
  lea rdi, [rel msg]
  call copystr
  pop rdi


  lea rax, [rel msg2]
  cmp rdi, 0
  cmove rdi, rax
  call copystr

  lea rdi, [rel msg3]
  call copystr

  ret 

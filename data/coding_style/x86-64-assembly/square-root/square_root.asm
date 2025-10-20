section .text
global square_root
square_root:
        ; X_(n+1)
        %define x_n edi

        ; c_n
        %define c_n eax
        xor c_n, c_n

        ; d_n
        %define d_n edx
        mov d_n, 1
        shl d_n, 30

        %define scratch_a ecx
        %define scratch_b esi
        %define scratch_c r8d

    .loop_d_bt_n:
        cmp d_n, x_n
        jbe .loop_d_bt_n_done
        shr d_n, 2
        jmp .loop_d_bt_n
    .loop_d_bt_n_done:

    .loop_d_ne_0:
        test d_n, d_n
        jz .loop_d_ne_0_done
        ; c + d
        mov scratch_a, c_n
        add scratch_a, d_n
        ; x >= c + d
        cmp x_n, scratch_a
        jge .loop_d_ne_0_if
        jmp .loop_d_ne_0_else
    .loop_d_ne_0_if_done:
        ; d >>= 2
        shr d_n, 2

        jmp .loop_d_ne_0

    .loop_d_ne_0_done:
ret

square_root.loop_d_ne_0_if:
        ; x -= c + d
        sub x_n, scratch_a
        ; c = (c >> 1) + d
        shr c_n, 1
        add c_n, d_n

        jmp square_root.loop_d_ne_0_if_done

square_root.loop_d_ne_0_else:
        ; c >>= 1
        shr c_n, 1

        jmp square_root.loop_d_ne_0_if_done

%ifidn __OUTPUT_FORMAT__,elf64
section .note.GNU-stack noalloc noexec nowrite progbits
%endif

; Taken from https://en.wikipedia.org/wiki/Square_root_algorithms#Basic_principle
; int32_t isqrt(int32_t n) {
;     assert(("sqrt input should be non-negative", n > 0));

;     // X_(n+1)
;     int32_t x = n;

;     // c_n
;     int32_t c = 0;

;     // d_n which starts at the highest power of four <= n
;     int32_t d = 1 << 30; // The second-to-top bit is set.
;                          // Same as ((unsigned) INT32_MAX + 1) / 2.
;     while (d > n) {
;         d >>= 2;
;     }

;     // for dₙ … d₀
;     while (d != 0) {
;         if (x >= c + d) {      // if X_(m+1) ≥ Y_m then a_m = 2^m
;             x -= c + d;        // X_m = X_(m+1) - Y_m
;             c = (c >> 1) + d;  // c_(m-1) = c_m/2 + d_m (a_m is 2^m)
;         }
;         else {
;             c >>= 1;           // c_(m-1) = c_m/2      (aₘ is 0)
;         }
;         d >>= 2;               // d_(m-1) = d_m/4
;     }
;     return c;                  // c_(-1)
; }

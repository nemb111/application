.text
.globl leap_year

// Macro to calculate modulo
// Parameters:
//    dividend [ro] - The number to be divided
//    divisor  [ro] - The number to divide by
//    res      [rw] - A temporary register to store intermediate and final results
// Returns:
//    res      [rw] - The result of dividend % divisor
.macro modulo dividend divisor res=x7
    udiv \res, \dividend , \divisor  // tmp = dividend / divisor
    msub \res, \res, \divisor, \dividend // res = dividend - tmp * divisor
.endm

leap_year:
        // x is leap year if, (x % 4 == 0) and ((x % 100 != 0) or (x % 400 == 0))

        mov x1, #4
        modulo x0, x1, x2 
        cmp x2, #0
        bne leap_year.false

        mov x1, #100
        modulo x0, x1, x2
        cmp x2, #0
        bne .true

        mov x1, #400
        modulo x0, x1, x2
        cmp x2, #0
        bne leap_year.false

    .true: 
        mov x0, #1 
        ret

leap_year.false:
    eor x0, x0, x0
    ret

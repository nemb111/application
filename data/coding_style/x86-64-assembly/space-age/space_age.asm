
section .rodata


%define SEC_PER_EARTH_YEAR              31557600
%define MERCURY_YEARS_PER_EARTH_YEAR    0.2408467
%define VENUS_YEARS_PER_EARTH_YEAR      0.61519726 
%define EARTH_YEARS_PER_EARTH_YEAR      1.0
%define MARS_YEARS_PER_EARTH_YEAR       1.8808158 
%define JUPITER_YEARS_PER_EARTH_YEAR    11.862615 
%define SATURN_YEARS_PER_EARTH_YEAR     29.447498 
%define URANUS_YEARS_PER_EARTH_YEAR     84.016846 
%define NEPTUNE_YEARS_PER_EARTH_YEAR    164.79132


planets: dq  MERCURY_YEARS_PER_EARTH_YEAR, \
             VENUS_YEARS_PER_EARTH_YEAR, \
             EARTH_YEARS_PER_EARTH_YEAR, \
             MARS_YEARS_PER_EARTH_YEAR,  \
             JUPITER_YEARS_PER_EARTH_YEAR, \
             SATURN_YEARS_PER_EARTH_YEAR, \
             URANUS_YEARS_PER_EARTH_YEAR, \
             NEPTUNE_YEARS_PER_EARTH_YEAR


section .text
global age
age:

    lea rcx, [rel planets]
    movsd xmm0, [rcx + rdi * 8]
    mov rax, SEC_PER_EARTH_YEAR
    CVTSI2SD xmm1, rax
    mulsd xmm0, xmm1
    CVTSI2SD xmm1, rsi
    divsd xmm1, xmm0
    CVTSD2SS xmm0, xmm1

    ret


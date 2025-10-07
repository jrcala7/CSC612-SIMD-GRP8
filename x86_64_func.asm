; assembly part using x86-64
bits 64
default rel

section .data
abs_mask dq 0x7FFFFFFFFFFFFFFF

section .text
global x86_vector_add

x86_vector_add:
    test rcx, rcx
    je .zero_return

    lea r8, [rdx + rcx*8]

    pxor xmm1, xmm1

    movq xmm2, qword [rel abs_mask]

.loop:
    movsd xmm0, qword [rdx]   
    andpd xmm0, xmm2          
    addsd xmm1, xmm0          
    add rdx, 8                
    cmp rdx, r8
    jne .loop

    movsd xmm0, xmm1
    ret

.zero_return:
    pxor xmm0, xmm0         
    ret

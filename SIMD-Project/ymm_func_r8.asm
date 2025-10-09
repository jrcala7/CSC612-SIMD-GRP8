; assembly part using SIMD ymm registers
section .data
varAll dq 0h7fffffffffffffff,0h7fffffffffffffff,0h7fffffffffffffff,0h7fffffffffffffff
varBound1 dq 0h7fffffffffffffff, 0h0, 0h0, 0h0
varBound2 dq 0h7fffffffffffffff, 0h7fffffffffffffff, 0h0, 0h0
varBound3 dq 0h7fffffffffffffff, 0h7fffffffffffffff, 0h7fffffffffffffff, 0h0


section .text

bits 64
default rel ; to handle address relocation

global ymm_vector_add_r8
ymm_vector_add_r8:
    vxorpd ymm0, ymm0, ymm0
    vxorpd ymm3, ymm3, ymm3
    vmovdqu ymm2, [varAll]

    lea r8, [rdx]
    mov rax, rcx
    shr rcx, 2    
        
L1:
    vmovdqu ymm1, [r8]
    vandpd ymm1, ymm1, [varAll]
    
    ;Add all values first vertically
    vaddpd ymm0, ymm0, ymm1
    
    add r8, 32
    loop L1


    and rax, 3
    jz fin

    ; Remainder

    vmovdqu ymm1, [r8]

    cmp rax, 1
    jnz m2 
    vandpd ymm1, ymm1, [varBound1]
    jmp bound

m2:
	cmp rax, 2
    jnz m3	
	vandpd ymm1, ymm1, [varBound2]
    jmp bound

m3:
	vandpd ymm1, ymm1, [varBound3]

bound:
    vaddpd ymm0, ymm0, ymm1

fin:    
    
    vhaddpd ymm0, ymm0, ymm0
    vpermpd ymm1, ymm0, 0b00_01_10_11
    vaddpd ymm0, ymm0, ymm1
        
    ret
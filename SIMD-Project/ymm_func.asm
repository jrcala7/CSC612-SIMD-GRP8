; assembly part using SIMD ymm registers
section .data
varAll dq 0h7fffffffffffffff,0h7fffffffffffffff,0h7fffffffffffffff,0h7fffffffffffffff
varBound dq 0h7fffffffffffffff, 0h0, 0h7fffffffffffffff, 0h0


section .text

bits 64
default rel ; to handle address relocation

global ymm_vector_add
ymm_vector_add:
    vxorpd ymm2, ymm2, ymm2
    vxorpd ymm3, ymm3, ymm3
    xor rax, rax

    vmovdqu ymm0, [varAll]
    vmovdqu ymm4, [varBound]

    shr rcx, 1    
    jnc lastShift
    inc rax

lastShift:
    shr rcx, 1
    jnc m1
    inc rax
    inc rax
    
m1:
	OR rax, 0
	jz L1

    dec rax
    jnz m2
	vshufpd ymm3, ymm4, ymm4, 0b0000_1110
    jmp L1

m2:
	dec rax
    jnz m3
	vshufpd ymm3, ymm4, ymm4, 0b0000_1100
	jmp L1

m3:
	vshufpd ymm3, ymm4, ymm4, 0b0000_1000

L1:
    vmovdqu ymm1, [rdx]
    vandpd ymm1, ymm0
    
    ;Add all values first vertically
    vaddpd ymm2, ymm1
    
    add rdx, 32
    loop L1

   ; cmp rax, -32
    ;je fin

    vmovdqu ymm1, [rdx] 
    vandpd ymm1, ymm3
    vaddpd ymm2, ymm1

fin:    
    
    vhaddpd ymm1, ymm2, ymm2
    vperm2f128 ymm2, ymm1, ymm1, 0b0_000_0_001
    vaddpd ymm2, ymm1
    
    vmovdqu ymm0, ymm2
    
    ret
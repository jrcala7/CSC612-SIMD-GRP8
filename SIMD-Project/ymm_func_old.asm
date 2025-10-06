; assembly part using SIMD ymm registers
section .data
varAll dq 0h7fffffffffffffff



section .text

bits 64
default rel ; to handle address relocation

global ymm_vector_add
ymm_vector_add:
	vbroadcastsd ymm0, [varAll]    
    shr rcx, 2
    vxorpd ymm2, ymm2, ymm2

L1:
    vmovdqu ymm1, [r8]
    vandpd ymm1, ymm0
    vaddpd ymm2, ymm1
    add r8, 32
    loop L1
    
    
fin:    
    ;pop rdx
    vhaddpd ymm3, ymm2, ymm2
   
    vperm2f128 ymm1, ymm3, ymm3, 0b0_000_0_001
    vaddpd ymm4, ymm1, ymm3
    
    vmovdqu [rdx], ymm4

    ;mov rcx, 1000000

;	sub rsp, 8*5 ; caller
;	lea rcx, [msg]
;	call printf
;	add rsp, 8*5
	ret

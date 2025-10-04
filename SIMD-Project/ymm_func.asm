; assembly part using SIMD ymm registers
section .data
varAll dq 0h7fffffffffffffff

varOne dq 0h7fffffffffffffff, 0h0, 0h0, 0h0
varTwo dq 0h7fffffffffffffff, 0h7fffffffffffffff, 0h0, 0h0
varThree dq 0h7fffffffffffffff,  0h7fffffffffffffff,  0h7fffffffffffffff,  0h0


msg db "Hello World YMM",13,10,0

section .text

bits 64
default rel ; to handle address relocation

global ymm_vector_add
extern printf

ymm_vector_add:
	vbroadcastsd ymm0, [varAll]    
	
	; For checking remainders later
	mov rax, rcx 
	
	; Round up way of getting the loop count, including the boundary values
    neg rcx
    sar rcx, 2
    neg rcx

L1:
    vmovdqu ymm1, [r8]
    vandpd ymm1, ymm0
    
    ; if last batch and has remainder, zero all non-numbers
    cmp rcx, 1
    jne addNums
    
    xor rdx,rdx
    mov rbx, 0h4
    idiv rbx
    
    cmp rdx, 0
    je addNums
    
removeOne:
    dec rdx
    jnz removeTwo
    
    vmovdqu ymm0, [varOne]
    vandpd ymm1, ymm0
    jmp addNums    

removeTwo:
    dec rdx
    jnz removeThree
    
    vmovdqu ymm0, [varTwo]
    vandpd ymm1, ymm0
    jmp addNums
    
removeThree:        
    
    vmovdqu ymm0, [varThree]
    vandpd ymm1, ymm0
    
    vextractf128 xmm5, ymm1, 1

addNums:    
    ; Add Values Horizontally First
    vhaddpd ymm2, ymm1, ymm1
    
    ; 
    vperm2f128 ymm3, ymm2, ymm2, 0b0_000_0_001
    vaddpd ymm3, ymm3, ymm2
    vaddpd ymm4, ymm3
    
    add r8, 32
    loop L1
    
    
fin:    
    vmovdqu [rdx], ymm4

;	sub rsp, 8*5 ; caller
;	lea rcx, [msg]
;	call printf
;	add rsp, 8*5
	ret

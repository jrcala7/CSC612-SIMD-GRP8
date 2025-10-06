; assembly part using SIMD ymm registers
section .data

varBound dq 0h7fffffffffffffff, 0h0, 0h7fffffffffffffff, 0h0
section .text

bits 64
default rel ; to handle address relocation

global buffer_asm
extern printf

buffer_asm:
	xor rax, rax
	vxorpd ymm0, ymm0, ymm0
	vmovdqu ymm0, [varBound]
	
    shr rcx, 1    
	jnc lastTest
	inc rax

lastTest:
	shr rcx, 1
	jnc m3
	inc rax
	inc rax

m3:
	cmp rax, 3
	jne m2
	vshufpd ymm0, ymm0, ymm0, 0b0000_1000
	jmp fin
m2:
	cmp rax, 2
	jne m1
	vshufpd ymm0, ymm0, ymm0, 0b0000_1100
	jmp fin
m1:
	cmp rax, 1
	jne fin
	vshufpd ymm0, ymm0, ymm0, 0b0000_1110

fin:
	ret

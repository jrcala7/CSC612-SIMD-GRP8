; assembly part using x86-64
section .data

msg db "Hello World XMM",13,10,0

section .text

bits 64
default rel ; to handle address relocation

global xmm_vector_add
extern printf

xmm_vector_add:
	sub rsp, 8*5 ; caller
	lea rcx, [msg]
	call printf
	add rsp, 8*5
	ret

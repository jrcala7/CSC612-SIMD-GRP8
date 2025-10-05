; assembly part using x86-64
section .data 
msg_abssum db "Answer = %.16f",10,0 
abs_mask dq 0x7fffffffffffffff, 0x7fffffffffffffff  

section .text

bits 64
default rel ; to handle address relocation

global xmm_vector_add
extern printf

xmm_vector_add:   
    mov r8, rdx  
    mov eax, ecx  
   ; mov ecx, eax
    shr ecx, 1    

    vxorpd xmm1, xmm1, xmm1
L1:
    cmp ecx, 0
    je LoopDone
    
    vmovdqu xmm0, [r8] 
    vandpd xmm0, xmm0, [abs_mask]
    vaddpd  xmm1, xmm1, xmm0  
                
    add r8, 16    
    dec ecx
    jmp L1 

LoopDone:
    mov eax, eax
    and eax, 1             
    cmp eax, 0
    je NoRemainder

    vmovdqu xmm0, [r8]   
    vandpd xmm0, xmm0, [abs_mask]
    vaddpd  xmm1, xmm1, xmm0     
    
NoRemainder:
    haddpd xmm1, xmm1 
    movapd xmm0, xmm1 

   ; SUB RSP, 8*5  
      ;  LEA RCX, [msg_abssum] 
     ;   movq rdx, xmm1  
    ;    CALL printf  
    
   ; ADD RSP, 8*5

	ret

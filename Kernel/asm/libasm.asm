GLOBAL cpuVendor
GLOBAL getKey
GLOBAL getReg
GLOBAL getMem

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

getKey:
	push rbp
	mov rbp, rsp

	push rdx

	mov rax, 0
	mov rdx, rdi
	in al, dx

	pop rdx
    
	leave
	ret

getReg:
    push rbp
    mov rbp, rsp
   
    cmp rdi, 0
    je .getRAX

    cmp rdi, 1
    je .getRBX

    cmp rdi, 2
    je .getRCX

    cmp rdi, 3
    je .getRDX

    cmp rdi, 4
    je .getRBP

    cmp rdi, 5
    je .getRDI

    cmp rdi, 6
    je .getRSI

    cmp rdi, 7
    je .getR8

    cmp rdi, 8
    je .getR9

    cmp rdi, 9
    je .getR10

    cmp rdi, 10
    je .getR11

    cmp rdi, 11
    je .getR12

    cmp rdi, 12
    je .getR13

    cmp rdi, 13
    je .getR14

    cmp rdi, 14
    je .getR15
  

    .exit:
        mov rsp, rbp
        pop rbp
        ret

    .getRAX:
        jmp .exit
	
    .getRBX:
        mov rax, rbx
        jmp .exit

   .getRCX:
        mov rax, rcx
        jmp .exit

   .getRDX:
        mov rax, rdx
        jmp .exit

    .getRBP:
        mov rax,rbp
         jmp .exit

    .getRDI:
        mov rax, rdi
        jmp .exit

   .getRSI:
        mov rax, rsi
        jmp .exit

    .getR8:
        mov rax, r8
        jmp .exit
    .getR9:
        mov rax, r9
        jmp .exit

    .getR10:
        mov rax, r10
        jmp .exit

    .getR11:
        mov rax, r11
        jmp .exit
    .getR12:
        mov rax, r12
        jmp .exit

    .getR13:
        mov rax, r13
        jmp .exit

    .getR14:
        mov rax, r14
        jmp .exit
    
    .getR15:
        mov rax, r15
        jmp .exit

getMem:
    push rbp
	mov rbp,rsp

	mov al, byte[rdi]

	mov rsp,rbp
	pop rbp
	ret
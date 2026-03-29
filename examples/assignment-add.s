section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	extern	puts
	global	f
f:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 0
	mov	r10, LS0
	mov	rdi, r10
	lea	r10, [rel puts]
	call	r10
	lea	r10, [rel a]
	mov	rax, r10
	jmp	.L0
	xor	rax, rax
.L0:
	add	rsp, 0
	pop	rbp
	ret

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 0
	mov	r10, 5
	lea	r11, [rel a]
	mov	qword [r11], r10
	lea	r10, [rel f]
	call	r10
	mov	r10, rax
	mov	r11, qword [r10]
	mov	r12, 15
	add	r11, r12
	mov	qword [r10], r11
	mov	r10, qword [rel a]
	mov	rdi, r10
	call	printi
.L1:
	xor	rax, rax
	add	rsp, 0
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0
	LS0: db 67, 97, 108, 108, 101, 100, 32, 39, 102, 40, 41, 39, 0

section .bss
	a: resb 8

section .note.GNU-stack


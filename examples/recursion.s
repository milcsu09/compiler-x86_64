section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	global	factorial
factorial:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	r8, qword [rbp-8]
	mov	r9, 0
	cmp	r8, r9
	setle	r8b
	test	r8b, r8b
	jz	.L1
	mov	r8, 1
	mov	rax, r8
	jmp	.L0
.L1:
	mov	r8, qword [rbp-8]
	push	r8
	mov	r10, qword [rbp-8]
	mov	r11, 1
	sub	r10, r11
	mov	rdi, r10
	sub	rsp, 8
	lea	r10, [rel factorial]
	call	r10
	add	rsp, 8
	pop	r8
	mov	r9, rax
	imul	r8, r9
	mov	rax, r8
	jmp	.L0
	xor	rax, rax
.L0:
	add	rsp, 16
	pop	rbp
	ret

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 0
	mov	r10, 5
	mov	rdi, r10
	lea	r10, [rel factorial]
	call	r10
	mov	r8, rax
	mov	rdi, r8
	call	printi
.L2:
	xor	rax, rax
	add	rsp, 0
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .bss

section .note.GNU-stack


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
	mov	r10, qword [rbp-8]
	mov	r11, 0
	cmp	r10, r11
	setle	r10b
	test	r10b, r10b
	jz	.L1
	mov	r10, 1
	mov	rax, r10
	jmp	.L0
.L1:
	mov	r10, qword [rbp-8]
	push	r10
	mov	r11, qword [rbp-8]
	mov	r12, 1
	sub	r11, r12
	mov	rdi, r11
	sub	rsp, 8
	lea	r11, [rel factorial]
	call	r11
	add	rsp, 8
	pop	r10
	mov	r11, rax
	imul	r10, r11
	mov	rax, r10
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
	mov	r10, rax
	mov	rdi, r10
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


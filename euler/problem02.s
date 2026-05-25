section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	r8, 1
	lea	r9, [rbp-8]
	mov	qword [r9], r8
	mov	r8, 2
	lea	r9, [rbp-16]
	mov	qword [r9], r8
	mov	r8, 0
	lea	r9, [rbp-24]
	mov	qword [r9], r8
	jmp	.L2
.L1:
	mov	r8, qword [rbp-8]
	mov	r9, 2
	mov	rax, r8
	cqo
	idiv	r9
	mov	r8, rdx
	mov	r9, 0
	cmp	r8, r9
	sete	r8b
	test	r8b, r8b
	jz	.L4
	lea	r8, [rbp-24]
	mov	r9, qword [r8]
	mov	r10, qword [rbp-8]
	add	r9, r10
	mov	qword [r8], r9
.L4:
	mov	r8, qword [rbp-8]
	mov	r9, qword [rbp-16]
	add	r8, r9
	lea	r9, [rbp-32]
	mov	qword [r9], r8
	mov	r8, qword [rbp-16]
	lea	r9, [rbp-8]
	mov	qword [r9], r8
	mov	r8, qword [rbp-32]
	lea	r9, [rbp-16]
	mov	qword [r9], r8
.L2:
	mov	r8, qword [rbp-8]
	mov	r9, 4000000
	cmp	r8, r9
	setl	r8b
	test	r8b, r8b
	jnz	.L1
.L3:
	mov	r8, qword [rbp-24]
	mov	rdi, r8
	call	printi
.L0:
	xor	rax, rax
	add	rsp, 32
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .bss

section .note.GNU-stack


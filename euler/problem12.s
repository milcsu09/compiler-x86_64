section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	global	isqrt
isqrt:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 48
	mov	qword [rbp-8], rdi
	mov	r8, 0
	lea	r9, [rbp-16]
	mov	qword [r9], r8
	mov	r8, qword [rbp-8]
	lea	r9, [rbp-24]
	mov	qword [r9], r8
	mov	r8, 0
	lea	r9, [rbp-32]
	mov	qword [r9], r8
	jmp	.L2
.L1:
	mov	r8, qword [rbp-16]
	mov	r9, qword [rbp-24]
	mov	r10, qword [rbp-16]
	sub	r9, r10
	mov	r10, 2
	mov	rax, r9
	cqo
	idiv	r10
	mov	r9, rax
	add	r8, r9
	lea	r9, [rbp-40]
	mov	qword [r9], r8
	mov	r8, qword [rbp-40]
	mov	r9, qword [rbp-40]
	imul	r8, r9
	mov	r9, qword [rbp-8]
	cmp	r8, r9
	setle	r8b
	test	r8b, r8b
	jz	.L4
	mov	r8, qword [rbp-40]
	lea	r9, [rbp-32]
	mov	qword [r9], r8
	mov	r8, qword [rbp-40]
	mov	r9, 1
	add	r8, r9
	lea	r9, [rbp-16]
	mov	qword [r9], r8
	jmp	.L5
.L4:
	mov	r8, qword [rbp-40]
	mov	r9, 1
	sub	r8, r9
	lea	r9, [rbp-24]
	mov	qword [r9], r8
.L5:
.L2:
	mov	r8, qword [rbp-16]
	mov	r9, qword [rbp-24]
	cmp	r8, r9
	setle	r8b
	test	r8b, r8b
	jnz	.L1
.L3:
	mov	r8, qword [rbp-32]
	mov	rax, r8
	jmp	.L0
	xor	rax, rax
.L0:
	add	rsp, 48
	pop	rbp
	ret

	global	n_divisors
n_divisors:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	qword [rbp-8], rdi
	mov	r8, 0
	lea	r9, [rbp-16]
	mov	qword [r9], r8
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	lea	r10, [rel isqrt]
	call	r10
	mov	r8, rax
	lea	r9, [rbp-32]
	mov	qword [r9], r8
	mov	r8, 1
	lea	r9, [rbp-24]
	mov	qword [r9], r8
	jmp	.L8
.L7:
	mov	r8, qword [rbp-8]
	mov	r9, qword [rbp-24]
	mov	rax, r8
	cqo
	idiv	r9
	mov	r8, rdx
	mov	r9, 0
	cmp	r8, r9
	sete	r8b
	test	r8b, r8b
	jz	.L11
	mov	r8, qword [rbp-8]
	mov	r9, qword [rbp-24]
	mov	rax, r8
	cqo
	idiv	r9
	mov	r8, rax
	mov	r9, qword [rbp-24]
	cmp	r8, r9
	sete	r8b
	test	r8b, r8b
	jz	.L12
	lea	r8, [rbp-16]
	mov	r9, qword [r8]
	mov	r10, 1
	add	r9, r10
	mov	qword [r8], r9
	jmp	.L13
.L12:
	lea	r8, [rbp-16]
	mov	r9, qword [r8]
	mov	r10, 2
	add	r9, r10
	mov	qword [r8], r9
.L13:
.L11:
.L10:
	lea	r8, [rbp-24]
	mov	r9, qword [r8]
	mov	r10, 1
	add	r9, r10
	mov	qword [r8], r9
.L8:
	mov	r8, qword [rbp-24]
	mov	r9, qword [rbp-32]
	cmp	r8, r9
	setle	r8b
	test	r8b, r8b
	jnz	.L7
.L9:
	mov	r8, qword [rbp-16]
	mov	rax, r8
	jmp	.L6
	xor	rax, rax
.L6:
	add	rsp, 32
	pop	rbp
	ret

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	r8, 1
	lea	r9, [rbp-8]
	mov	qword [r9], r8
	mov	r8, 2
	lea	r9, [rbp-16]
	mov	qword [r9], r8
	jmp	.L16
.L15:
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	lea	r10, [rel n_divisors]
	call	r10
	mov	r8, rax
	mov	r9, 500
	cmp	r8, r9
	setge	r8b
	test	r8b, r8b
	jz	.L18
	mov	r8, qword [rbp-8]
	mov	rdi, r8
	call	printi
	jmp	.L17
.L18:
	lea	r8, [rbp-8]
	mov	r9, qword [r8]
	mov	r10, qword [rbp-16]
	add	r9, r10
	mov	qword [r8], r9
	lea	r8, [rbp-16]
	mov	r9, qword [r8]
	mov	r10, 1
	add	r9, r10
	mov	qword [r8], r9
.L16:
	mov	r8, 1
	test	r8, r8
	jnz	.L15
.L17:
.L14:
	xor	rax, rax
	add	rsp, 16
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .bss

section .note.GNU-stack


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
	mov	r10, 0
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	mov	r10, qword [rbp-8]
	lea	r11, [rbp-24]
	mov	qword [r11], r10
	mov	r10, 0
	lea	r11, [rbp-32]
	mov	qword [r11], r10
	jmp	.L2
.L1:
	mov	r10, qword [rbp-16]
	mov	r11, qword [rbp-24]
	mov	r12, qword [rbp-16]
	sub	r11, r12
	mov	r12, 2
	mov	rax, r11
	cqo
	idiv	r12
	mov	r11, rax
	add	r10, r11
	lea	r11, [rbp-40]
	mov	qword [r11], r10
	mov	r10, qword [rbp-40]
	mov	r11, qword [rbp-40]
	imul	r10, r11
	mov	r11, qword [rbp-8]
	cmp	r10, r11
	setle	r10b
	test	r10b, r10b
	jz	.L4
	mov	r10, qword [rbp-40]
	lea	r11, [rbp-32]
	mov	qword [r11], r10
	mov	r10, qword [rbp-40]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	jmp	.L5
.L4:
	mov	r10, qword [rbp-40]
	mov	r11, 1
	sub	r10, r11
	lea	r11, [rbp-24]
	mov	qword [r11], r10
.L5:
.L2:
	mov	r10, qword [rbp-16]
	mov	r11, qword [rbp-24]
	cmp	r10, r11
	setle	r10b
	test	r10b, r10b
	jnz	.L1
.L3:
	mov	r10, qword [rbp-32]
	mov	rax, r10
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
	mov	r10, 0
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	lea	r10, [rel isqrt]
	call	r10
	mov	r10, rax
	lea	r11, [rbp-32]
	mov	qword [r11], r10
	mov	r10, 1
	lea	r11, [rbp-24]
	mov	qword [r11], r10
	jmp	.L8
.L7:
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-24]
	mov	rax, r10
	cqo
	idiv	r11
	mov	r10, rdx
	mov	r11, 0
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L11
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-24]
	mov	rax, r10
	cqo
	idiv	r11
	mov	r10, rax
	mov	r11, qword [rbp-24]
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L12
	mov	r10, qword [rbp-16]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	jmp	.L13
.L12:
	mov	r10, qword [rbp-16]
	mov	r11, 2
	add	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
.L13:
.L11:
.L10:
	mov	r10, qword [rbp-24]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-24]
	mov	qword [r11], r10
.L8:
	mov	r10, qword [rbp-24]
	mov	r11, qword [rbp-32]
	cmp	r10, r11
	setle	r10b
	test	r10b, r10b
	jnz	.L7
.L9:
	mov	r10, qword [rbp-16]
	mov	rax, r10
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
	mov	r10, 1
	lea	r11, [rbp-8]
	mov	qword [r11], r10
	mov	r10, 2
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	jmp	.L16
.L15:
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	lea	r10, [rel n_divisors]
	call	r10
	mov	r10, rax
	mov	r11, 500
	cmp	r10, r11
	setge	r10b
	test	r10b, r10b
	jz	.L18
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	call	printi
	jmp	.L17
.L18:
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	add	r10, r11
	lea	r11, [rbp-8]
	mov	qword [r11], r10
	mov	r10, qword [rbp-16]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
.L16:
	mov	r10, 1
	test	r10, r10
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


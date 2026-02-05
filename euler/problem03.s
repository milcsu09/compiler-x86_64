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

	global	isprime
isprime:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	r10, 2
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	jmp	.L8
.L7:
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	mov	rax, r10
	cqo
	idiv	r11
	mov	r10, rdx
	mov	r11, 0
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L11
	mov	r10, 0
	mov	al, r10b
	jmp	.L6
.L11:
.L10:
	mov	r10, qword [rbp-16]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
.L8:
	mov	r10, qword [rbp-16]
	push	r10
	mov	r11, qword [rbp-8]
	mov	rdi, r11
	sub	rsp, 8
	lea	r11, [rel isqrt]
	call	r11
	add	rsp, 8
	pop	r10
	mov	r11, rax
	mov	r12, 1
	add	r11, r12
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L7
.L9:
	mov	r10, 1
	mov	al, r10b
	jmp	.L6
	xor	rax, rax
.L6:
	add	rsp, 16
	pop	rbp
	ret

	global	biggest_prime_factor
biggest_prime_factor:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	qword [rbp-8], rdi
	mov	r10, 2
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	mov	r10, 0
	lea	r11, [rbp-24]
	mov	qword [r11], r10
	jmp	.L14
.L13:
	mov	r10, qword [rbp-16]
	mov	rdi, r10
	lea	r10, [rel isprime]
	call	r10
	mov	r10b, al
	test	r10b, r10b
	jz	.L17
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	mov	rax, r10
	cqo
	idiv	r11
	mov	r10, rdx
	mov	r11, 0
	cmp	r10, r11
	sete	r10b
.L17:
	test	r10b, r10b
	jz	.L16
	mov	r10, qword [rbp-16]
	mov	r11, qword [rbp-24]
	cmp	r10, r11
	setg	r10b
	test	r10b, r10b
	jz	.L18
	mov	r10, qword [rbp-16]
	lea	r11, [rbp-24]
	mov	qword [r11], r10
.L18:
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	mov	rax, r10
	cqo
	idiv	r11
	mov	r10, rax
	lea	r11, [rbp-8]
	mov	qword [r11], r10
.L16:
	mov	r10, qword [rbp-16]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
.L14:
	mov	r10, qword [rbp-8]
	mov	r11, 1
	cmp	r10, r11
	setne	r10b
	test	r10b, r10b
	jnz	.L13
.L15:
	mov	r10, qword [rbp-24]
	mov	rax, r10
	jmp	.L12
	xor	rax, rax
.L12:
	add	rsp, 32
	pop	rbp
	ret

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 0
	mov	r10, 600851475143
	mov	rdi, r10
	lea	r10, [rel biggest_prime_factor]
	call	r10
	mov	r10, rax
	mov	rdi, r10
	call	printi
.L19:
	xor	rax, rax
	add	rsp, 0
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .note.GNU-stack


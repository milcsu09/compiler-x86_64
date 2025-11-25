section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	global	mandelbrot
mandelbrot:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 80

	mov	qword [rbp-8], rdi
	mov	qword [rbp-16], rsi
	mov	qword [rbp-24], rdx
	mov	r10, 0
	lea	r11, [rbp-32]
	mov	qword [r11], r10
	mov	r10, 0
	lea	r11, [rbp-40]
	mov	qword [r11], r10
	mov	r10, 0
	lea	r11, [rbp-48]
	mov	qword [r11], r10
	jmp	.L2
.L1:
	mov	r10, qword [rbp-32]
	mov	r11, qword [rbp-32]
	imul	r10, r11
	mov	r11, 16777216
	mov	rax, r10
	cqo
	idiv	r11
	mov	r10, rax
	lea	r11, [rbp-56]
	mov	qword [r11], r10
	mov	r10, qword [rbp-40]
	mov	r11, qword [rbp-40]
	imul	r10, r11
	mov	r11, 16777216
	mov	rax, r10
	cqo
	idiv	r11
	mov	r10, rax
	lea	r11, [rbp-64]
	mov	qword [r11], r10
	mov	r10, qword [rbp-56]
	mov	r11, qword [rbp-64]
	add	r10, r11
	mov	r11, 4
	mov	r12, 16777216
	imul	r11, r12
	cmp	r10, r11
	setg	r10b
	test	r10b, r10b
	jz	.L4
	jmp	.L3
.L4:
	mov	r10, 2
	mov	r11, qword [rbp-32]
	imul	r10, r11
	mov	r11, qword [rbp-40]
	imul	r10, r11
	mov	r11, 16777216
	mov	rax, r10
	cqo
	idiv	r11
	mov	r10, rax
	mov	r11, qword [rbp-16]
	add	r10, r11
	lea	r11, [rbp-72]
	mov	qword [r11], r10
	mov	r10, qword [rbp-56]
	mov	r11, qword [rbp-64]
	sub	r10, r11
	mov	r11, qword [rbp-8]
	add	r10, r11
	lea	r11, [rbp-80]
	mov	qword [r11], r10
	mov	r10, qword [rbp-80]
	lea	r11, [rbp-32]
	mov	qword [r11], r10
	mov	r10, qword [rbp-72]
	lea	r11, [rbp-40]
	mov	qword [r11], r10
	mov	r10, qword [rbp-48]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-48]
	mov	qword [r11], r10
.L2:
	mov	r10, qword [rbp-48]
	mov	r11, qword [rbp-24]
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L1
.L3:
	mov	r10, qword [rbp-48]
	mov	rax, r10
	jmp	.L0

.L0:
	add	rsp, 80
	pop	rbp
	ret

	extern	putchar
	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 96

	mov	r10, 33554432
	neg	r10
	lea	r11, [rbp-8]
	mov	qword [r11], r10
	mov	r10, 16777216
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	mov	r10, 25165824
	neg	r10
	lea	r11, [rbp-24]
	mov	qword [r11], r10
	mov	r10, 25165824
	lea	r11, [rbp-32]
	mov	qword [r11], r10
	mov	r10, qword [rbp-16]
	mov	r11, qword [rbp-8]
	sub	r10, r11
	mov	r11, 80
	mov	rax, r10
	cqo
	idiv	r11
	mov	r10, rax
	lea	r11, [rbp-40]
	mov	qword [r11], r10
	mov	r10, qword [rbp-32]
	mov	r11, qword [rbp-24]
	sub	r10, r11
	mov	r11, 40
	mov	rax, r10
	cqo
	idiv	r11
	mov	r10, rax
	lea	r11, [rbp-48]
	mov	qword [r11], r10
	mov	r10, LS0
	lea	r11, [rbp-56]
	mov	qword [r11], r10
	mov	r10, 0
	lea	r11, [rbp-64]
	mov	qword [r11], r10
	jmp	.L7
.L6:
	mov	r10, qword [rbp-32]
	mov	r11, qword [rbp-64]
	mov	r12, qword [rbp-48]
	imul	r11, r12
	sub	r10, r11
	lea	r11, [rbp-80]
	mov	qword [r11], r10
	mov	r10, 0
	lea	r11, [rbp-72]
	mov	qword [r11], r10
	jmp	.L11
.L10:
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-72]
	mov	r12, qword [rbp-40]
	imul	r11, r12
	add	r10, r11
	lea	r11, [rbp-88]
	mov	qword [r11], r10
	mov	r10, qword [rbp-88]
	mov	rdi, r10
	mov	r10, qword [rbp-80]
	mov	rsi, r10
	mov	r10, 15
	mov	rdx, r10
	lea	r10, [rel mandelbrot]
	call	r10
	mov	r10, rax
	lea	r11, [rbp-96]
	mov	qword [r11], r10
	mov	r10, qword [rbp-56]
	mov	r11, qword [rbp-96]
	mov	r12, 9
	imul	r11, r12
	mov	r12, 15
	mov	rax, r11
	cqo
	idiv	r12
	mov	r11, rax
	add	r10, r11
	mov	r10b, byte [r10]
	mov	dil, r10b
	lea	r10, [rel putchar]
	call	r10
.L13:
	mov	r10, qword [rbp-72]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-72]
	mov	qword [r11], r10
.L11:
	mov	r10, qword [rbp-72]
	mov	r11, 80
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L10
.L12:
	mov	r10, 10
	mov	dil, r10b
	lea	r10, [rel putchar]
	call	r10
.L9:
	mov	r10, qword [rbp-64]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-64]
	mov	qword [r11], r10
.L7:
	mov	r10, qword [rbp-64]
	mov	r11, 40
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L6
.L8:
	mov	r10, 0
	mov	eax, r10d
	jmp	.L5

.L5:
	add	rsp, 96
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0
	LS0: db 32, 46, 58, 45, 61, 43, 42, 35, 37, 64, 0

section .note.GNU-stack


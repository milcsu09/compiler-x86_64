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
	mov	r8, 0
	lea	r9, [rbp-32]
	mov	qword [r9], r8
	mov	r8, 0
	lea	r9, [rbp-40]
	mov	qword [r9], r8
	mov	r8, 0
	lea	r9, [rbp-48]
	mov	qword [r9], r8
	jmp	.L2
.L1:
	mov	r8, qword [rbp-32]
	mov	r9, qword [rbp-32]
	imul	r8, r9
	mov	r9, 16777216
	mov	rax, r8
	cqo
	idiv	r9
	mov	r8, rax
	lea	r9, [rbp-56]
	mov	qword [r9], r8
	mov	r8, qword [rbp-40]
	mov	r9, qword [rbp-40]
	imul	r8, r9
	mov	r9, 16777216
	mov	rax, r8
	cqo
	idiv	r9
	mov	r8, rax
	lea	r9, [rbp-64]
	mov	qword [r9], r8
	mov	r8, qword [rbp-56]
	mov	r9, qword [rbp-64]
	add	r8, r9
	mov	r9, 67108864
	cmp	r8, r9
	setg	r8b
	test	r8b, r8b
	jz	.L4
	jmp	.L3
.L4:
	mov	r8, 2
	mov	r9, qword [rbp-32]
	imul	r8, r9
	mov	r9, qword [rbp-40]
	imul	r8, r9
	mov	r9, 16777216
	mov	rax, r8
	cqo
	idiv	r9
	mov	r8, rax
	mov	r9, qword [rbp-16]
	add	r8, r9
	lea	r9, [rbp-72]
	mov	qword [r9], r8
	mov	r8, qword [rbp-56]
	mov	r9, qword [rbp-64]
	sub	r8, r9
	mov	r9, qword [rbp-8]
	add	r8, r9
	lea	r9, [rbp-80]
	mov	qword [r9], r8
	mov	r8, qword [rbp-80]
	lea	r9, [rbp-32]
	mov	qword [r9], r8
	mov	r8, qword [rbp-72]
	lea	r9, [rbp-40]
	mov	qword [r9], r8
	lea	r8, [rbp-48]
	mov	r9, qword [r8]
	mov	r10, 1
	add	r9, r10
	mov	qword [r8], r9
.L2:
	mov	r8, qword [rbp-48]
	mov	r9, qword [rbp-24]
	cmp	r8, r9
	setl	r8b
	test	r8b, r8b
	jnz	.L1
.L3:
	mov	r8, qword [rbp-48]
	mov	rax, r8
	jmp	.L0
	xor	rax, rax
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
	mov	r8, -33554432
	lea	r9, [rbp-8]
	mov	qword [r9], r8
	mov	r8, 16777216
	lea	r9, [rbp-16]
	mov	qword [r9], r8
	mov	r8, -25165824
	lea	r9, [rbp-24]
	mov	qword [r9], r8
	mov	r8, 25165824
	lea	r9, [rbp-32]
	mov	qword [r9], r8
	mov	r8, qword [rbp-16]
	mov	r9, qword [rbp-8]
	sub	r8, r9
	mov	r9, 80
	mov	rax, r8
	cqo
	idiv	r9
	mov	r8, rax
	lea	r9, [rbp-40]
	mov	qword [r9], r8
	mov	r8, qword [rbp-32]
	mov	r9, qword [rbp-24]
	sub	r8, r9
	mov	r9, 40
	mov	rax, r8
	cqo
	idiv	r9
	mov	r8, rax
	lea	r9, [rbp-48]
	mov	qword [r9], r8
	mov	r8, LS0
	lea	r9, [rbp-56]
	mov	qword [r9], r8
	mov	r8, 0
	lea	r9, [rbp-64]
	mov	qword [r9], r8
	jmp	.L7
.L6:
	mov	r8, qword [rbp-32]
	mov	r9, qword [rbp-64]
	mov	r10, qword [rbp-48]
	imul	r9, r10
	sub	r8, r9
	lea	r9, [rbp-80]
	mov	qword [r9], r8
	mov	r8, 0
	lea	r9, [rbp-72]
	mov	qword [r9], r8
	jmp	.L11
.L10:
	mov	r8, qword [rbp-8]
	mov	r9, qword [rbp-72]
	mov	r10, qword [rbp-40]
	imul	r9, r10
	add	r8, r9
	lea	r9, [rbp-88]
	mov	qword [r9], r8
	mov	r10, qword [rbp-88]
	mov	rdi, r10
	mov	r10, qword [rbp-80]
	mov	rsi, r10
	mov	r10, 15
	mov	rdx, r10
	lea	r10, [rel mandelbrot]
	call	r10
	mov	r8, rax
	lea	r9, [rbp-96]
	mov	qword [r9], r8
	mov	r10, qword [rbp-56]
	mov	r11, qword [rbp-96]
	push	r8
	mov	r8, 9
	imul	r11, r8
	pop	r8
	push	r8
	mov	r8, 15
	mov	rax, r11
	cqo
	idiv	r8
	mov	r11, rax
	pop	r8
	add	r10, r11
	mov	r10b, byte [r10]
	mov	dil, r10b
	lea	r10, [rel putchar]
	call	r10
.L13:
	lea	r8, [rbp-72]
	mov	r9, qword [r8]
	mov	r10, 1
	add	r9, r10
	mov	qword [r8], r9
.L11:
	mov	r8, qword [rbp-72]
	mov	r9, 80
	cmp	r8, r9
	setl	r8b
	test	r8b, r8b
	jnz	.L10
.L12:
	mov	r10, 10
	mov	dil, r10b
	lea	r10, [rel putchar]
	call	r10
.L9:
	lea	r8, [rbp-64]
	mov	r9, qword [r8]
	mov	r10, 1
	add	r9, r10
	mov	qword [r8], r9
.L7:
	mov	r8, qword [rbp-64]
	mov	r9, 40
	cmp	r8, r9
	setl	r8b
	test	r8b, r8b
	jnz	.L6
.L8:
.L5:
	xor	rax, rax
	add	rsp, 96
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0
	LS0: db 32, 46, 58, 45, 61, 43, 42, 35, 37, 64, 0

section .bss

section .note.GNU-stack

